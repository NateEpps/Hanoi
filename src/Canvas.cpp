//
// Canvas.cpp
// Nathanael Epps
//

#include "Canvas.hpp"

#include "Sansation.hpp"

using namespace sf;

#include <cmath>
#ifdef DEBUG
# include <iostream>
using std::cout;
#endif

static Canvas* last = nullptr;
static sf::Font sansation;

Canvas::Canvas(size_t num, sf::RenderWindow& pw) : N(num)
{
    // Print();
    
    last = this;

    static bool init = false;
    if (!init) {
        if (!sansation.loadFromMemory(GetSansationData(), GetAmtSansationBytes()))
            throw MakeException("Could not load font");
        init = true;
    }
    
    parent = &pw;
    current = nullptr;
#ifdef DEBUG
    AllMoves = GetHanoiMovesAsync(N, cout);
    cout << "(Constructor) Moves: " << AllMoves << "\n";
    cout << "(Constructor) Amount of Moves: " << AllMoves.size() / 2 << "\n";
#else
    AllMoves = GetHanoiMoves(N);
#endif
    // set up onscreen text
    label.setFont(sansation);
    label.setPosition(20, 10);
    label.setCharacterSize(parent->getSize().x * 0.02);
    label.setString("Rings: " + lexical_cast<string>(N) + "\n" +
                    "Moves Left: " + lexical_cast<string>(AllMoves.size()/2) + "\nTotal Moves: " + lexical_cast<string>(AmtHanoiMoves(N)));
    
    // set up poles
    const auto wsize = parent->getSize();
    for (auto& shape : poles) {
        shape.setSize(Vector2f(wsize.x * 0.03, wsize.y * 0.80));
        shape.setFillColor(Color(128, 65, 65)); // brown ish?
        shape.setPosition(shape.getPosition().x,
                          wsize.y * 0.20);
    }
    
    poles[1].setPosition(wsize.x / 2, poles[1].getPosition().y);
    poles[1].move(-1 * poles[1].getGlobalBounds().width / 2, 0);
    
    const auto MaxRingWidth = Ring::getMaxRingWidth(wsize.y);
    
    poles[0].setPosition(wsize.x / 2 - MaxRingWidth,
                         poles[0].getPosition().y);
    poles[0].move(-1 * poles[0].getGlobalBounds().width / 2, 0);
    
    poles[2].setPosition(wsize.x / 2 + MaxRingWidth,
                         poles[2].getPosition().y);
    poles[2].move(-1 * poles[2].getGlobalBounds().width / 2, 0);
    
    // move further apart
    poles[0].move(wsize.y * -0.3, 0);
    poles[2].move(wsize.y *  0.3, 0);
    
    // set up rings
    for (size_t index = 1; index <= N; index++) {
        // add the ring to the list
        rings.emplace_back(index, N);
        
        // set it's position
        auto& r = rings.back();
        Vector2f pos;
        
        pos.x = poles[0].getPosition().x - std::abs(r.getSize().x/2 - poles[0].getSize().x/2);
        
        pos.y = parent->getSize().y - ((N - index + 1) * r.getSize().y);
        
        r.setPosition(pos);
    }
}

void Canvas::update(sf::Time dt)
{
    label.setString("Rings: " + lexical_cast<string>(N) + "\nMoves Left: " + lexical_cast<string>(AllMoves.size()/2) + "\nTotal Moves: " + lexical_cast<string>(AmtHanoiMoves(N)));
    
    if (AllMoves.empty() && !current)
        return;
    
    if (!current)
        DoMove(); // should update the current pointer
    
    if (!current)
        throw MakeException("Nullptr");
    
    current->update(dt);
    if (current->isDone()) {
        current = nullptr;
    }
    
    /*if (current) {
        current->update(dt);
        if (current->isDone()) {
            current = nullptr;
            DoMove();
        }
    }
    else {
        DoMove();
        if (current)
            current->update(dt);
        else
            throw MakeException("Nullptr");
    }*/
}

/*if (isDone())
 return;
 
 if (current) {
 current->update(dt);
 if (current->isDone()) {
 current = nullptr;
 DoMove();
 }
 }
 else {
 DoMove();
 if (current)
 current->update(dt);
 else
 throw MakeException("Nullptr");
 }*/

bool Canvas::isDone() const
{
    return AllMoves.empty() || AllMoves == "";
}

void Canvas::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const auto& p : poles)
        target.draw(p, states);
    for (const auto& r : rings)
        target.draw(r, states);
    
    target.draw(label, states);
}

/*static*/ Vector2u Canvas::getWindowBounds()
{
    if (last)
        return last->parent->getSize();
    else
        throw MakeException("Nullptr");
}

void Canvas::DoMove()
{
    // Print();
    // cout << "(DoMove) Moves: " << AllMoves << "\n";
    
    if (AllMoves.empty()) return;
    
    // store the move in a local variable,
    // take it off the list of all moves
    string move = AllMoves.substr(0, 2);
    AllMoves = AllMoves.substr(2);
    
    // cout << "(DoMove) move (local): " << move << "\n";
    // cout << "(DoMove) AllMoves (instance): " << AllMoves << "\n\n";
    
    current = GetRingsOnPole(lexical_cast<size_t>(move.front())).front();
    auto& pole = poles[lexical_cast<size_t>(move.back()) - 1];
    
    // compute some positions
    const float OldXPos = current->getPosition().x;
    static const float TopYLevel = parent->getSize().y * 0.1;
    float DestXPos = pole.getPosition().x - (current->getSize().x/2 - pole.getSize().x/2);
    
    auto amt = GetAmtRingsOnPole(lexical_cast<size_t>(move.back()));
    float DestYPos = 0;
    if (amt == 0)
        DestYPos = getWindowBounds().y - current->getSize().y;
    else
        DestYPos = getWindowBounds().y - (current->getSize().y * (amt + 1));
    
    current->setPath({OldXPos, TopYLevel},
                     {DestXPos, TopYLevel},
                     {DestXPos, DestYPos});
}

/*
 getWindowBounds().y - (current->getSize().y * (amt))
 
 float DestYPos = 0;
 if (amtRingsOnDestPole == 0)
 DestYPos = getWindowBounds().y - ringHeight;
 else
 DestYPos = getWindowBounds().y - (amtRingsOnDestPole) * ringHeight;*/
/*
 cout << "Window height: " << getWindowBounds().y << "\n";
 cout << "AmtRingsOnDestPole: " << amtRingsOnDestPole << "\n";
 cout << "ring height: " << ringHeight << "\n";
 cout << "Amt subtract: " << (amtRingsOnDestPole - 1) * ringHeight << "\n";
 cout << "DestYPos: " << DestYPos << "\n";
 */

size_t Canvas::GetAmtRingsOnPole(size_t s)
{
    // Print();
    // std::cout << "Pole num: " << s << '\n';
    
    if (s < 1 || s > 3)
        throw MakeException("Out of bounds");
    if (rings.empty())
        return 0;
    
    auto& pole = poles[s - 1];
    
    size_t amt = 0;
    for (Ring& r : rings)
        if (r.getGlobalBounds().intersects(pole.getGlobalBounds()))
            amt++;
    
    return amt;
}

std::vector<Ring*> Canvas::GetRingsOnPole(size_t s)
{
    // Print();
    // std::cout << "Pole num: " << s << '\n';
    
    if (s < 1 || s > 3)
        throw MakeException("Out of bounds");
    if (rings.empty())
        return std::vector<Ring*>();
    
    auto& pole = poles[s - 1];
    
    std::vector<Ring*> onpole;
    for (Ring& r : rings)
        if (r.getGlobalBounds().intersects(pole.getGlobalBounds()))
            onpole.push_back(&r);
    
    if (onpole.empty())
        throw MakeException("Empty \'onpole\' vector: pole " + lexical_cast<string>(s - 1));
    
    std::sort(onpole.begin(), onpole.end(), [](Ring* left, Ring* right){
        return left->getPosition().y < right->getPosition().y;
    });
    
    return onpole;
}
