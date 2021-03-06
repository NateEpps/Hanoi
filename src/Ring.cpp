//
// Ring.cpp
// Nathanael Epps
//

#include "Ring.hpp"
#include "Util.hpp"
#include "Canvas.hpp"
#include "Sansation.hpp"

using namespace sf;

static constexpr auto MaxPercent = 0.2;
static Font sansation;

Ring::Ring(size_t num, size_t max)
{
    // Print();
    
    static bool init = false;
    if (!init) {
        if (!sansation.loadFromMemory(GetSansationData(), GetAmtSansationBytes()))
            throw MakeException("Could not load font");
        init = true;
    }
    
    setPosition(0, 0);
    
    auto wbounds = Canvas::getWindowBounds();
    auto width = Scale(wbounds.y * 0.1, wbounds.y * 0.4, max, num - 1);
    
    mshape.setSize(sf::Vector2f(width, wbounds.x * 0.036));
    
    mshape.setFillColor(GetRandColor());
    
    label.setString(lexical_cast<string>(num));
    label.setFont(sansation);
    label.setCharacterSize(getSize().y * 0.75);
}

constexpr int SpeedFactor = 6000; // default 6000
constexpr int Tolerance = 5; // default 5

void Ring::update(sf::Time dt)
{
    if (points.empty())
        return;
    
    auto dest = points.front();
    
    Vector2f vec {0, 0};
    if (IsClose(getPosition().x, dest.x, Tolerance))
        vec.x = 0;
    else if (getPosition().x < dest.x)
        vec.x = 1;
    else if (getPosition().x > dest.x)
        vec.x = -1;
    
    if (IsClose(getPosition().y, dest.y, Tolerance))
        vec.y = 0;
    else if (getPosition().y < dest.y)
        vec.y = 1;
    else if (getPosition().y > dest.y)
        vec.y = -1;
    
    vec.x *= SpeedFactor * dt.asSeconds();
    vec.y *= SpeedFactor * dt.asSeconds();
    
    move(vec);
    
    if (IsClose(getPosition(), Vector2f(dest), Tolerance) && !points.empty())        
        points.pop_front();
    
    /* std::cout << "Path: " << points << "\n";
    std::cout << "Position: " << getPosition() << "\n";
    std::cout << "Tolerance: " << Tolerance << "\n\n"; */
}

bool Ring::isDone() const
{
    return points.empty();
}

void Ring::setPath(sf::Vector2f one, sf::Vector2f two, sf::Vector2f three)
{
    /* one.x = (float) Round(one.x);
    two.x = (float) Round(two.x);
    three.x = (float) Round(three.x);

    one.y = (float) Round(one.y);
    two.y = (float) Round(two.y);
    three.y = (float) Round(three.y); */
    
    points.push_back(one);
    points.push_back(two);
    points.push_back(three);
}

Vector2f Ring::getDestination() const
{
    if (points.empty())
        throw MakeException("Empty deque");
    else
        return points.back();
}

FloatRect Ring::getGlobalBounds() const
{
    return mshape.getGlobalBounds();
}

Vector2f Ring::getSize() const
{
    return mshape.getSize();
}

void Ring::setPosition(float x, float y)
{
    mshape.setPosition(x, y);
    
    label.setPosition(mshape.getPosition().x,
                      mshape.getPosition().y);
    label.move(mshape.getSize().x / 2,
               mshape.getSize().y / 2);
    label.move(-0.5  * label.getGlobalBounds().width,
               -0.75 * label.getGlobalBounds().height);
}

void Ring::setPosition(sf::Vector2f v)
{
    setPosition(v.x, v.y);
}

sf::Vector2f Ring::getPosition() const
{
    return mshape.getPosition();
}

void Ring::move(sf::Vector2f v)
{
    setPosition(getPosition().x + v.x, getPosition().y + v.y);
}

void Ring::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mshape);
    target.draw(label);
}

size_t Ring::getMaxRingWidth(size_t windowWidth)
{
    return MaxPercent * windowWidth;
}
