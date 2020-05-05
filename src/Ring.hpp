//
// Ring.hpp
// Nathanael Epps
//

#ifndef RING_HPP
#define RING_HPP

#include <deque>
#include <SFML/Graphics.hpp>

class Ring : public sf::Drawable
{
public:
    Ring(size_t, size_t);
    
    void update(sf::Time);
    
    bool isDone() const;
    
    void setPath(sf::Vector2f, sf::Vector2f, sf::Vector2f);
    
    sf::Vector2f getDestination() const;
    
    sf::FloatRect getGlobalBounds() const;
    sf::Vector2f getSize() const;
    
    void setPosition(float, float);
    void setPosition(sf::Vector2f);
    sf::Vector2f getPosition() const;
    
    void move(sf::Vector2f);
    
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const override;
    
    static size_t getMaxRingWidth(size_t);
private:
    sf::RectangleShape mshape;
    sf::Text label;
    
    std::deque<sf::Vector2f> points;
};

#endif
