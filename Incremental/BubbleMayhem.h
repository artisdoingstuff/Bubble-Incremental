#include "Includes.h"

struct BubbleMayhem
{
    sf::RectangleShape hitbox;
    sf::Vector2f position;
    sf::Clock bubbleMayhemLifetimeClock;
    float bubbleLifetime;

    BubbleMayhem(sf::Vector2f position, sf::Vector2f size = { 75.0f, 75.0f }, float lifeTimeSec = 4.0f)
    {
        hitbox.setSize(size);
        hitbox.setPosition(position);
        hitbox.setFillColor(sf::Color::Cyan);
        bubbleLifetime = lifeTimeSec;
    }

    bool isTimeExpired() const
    {
        return bubbleMayhemLifetimeClock.getElapsedTime().asSeconds() >= bubbleLifetime;
    }
};