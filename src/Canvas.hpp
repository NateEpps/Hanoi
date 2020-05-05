//
// Canvas.hpp
// Nathanael Epps
//

#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include "Util.hpp"
#include "Ring.hpp"

class Canvas : public sf::Drawable
{
public:
    Canvas(size_t, sf::RenderWindow&);
    
    void update(sf::Time);
    
    bool isDone() const;
    
    virtual void draw(sf::RenderTarget&, sf::RenderStates states) const override;
    
    static sf::Vector2u getWindowBounds();
    
private:
    // methods
    void DoMove();
    
    size_t GetAmtRingsOnPole(size_t);
    
    std::vector<Ring*> GetRingsOnPole(size_t);
    
    // variables
    sf::RectangleShape poles[3];
    string AllMoves;
    const size_t N;
    sf::RenderWindow* parent;
    std::vector<Ring> rings;
    Ring* current;
    sf::Text label;
};

#endif
