#include "Includes.h"

struct BubbleMayhem
{
    sf::RectangleShape hitbox;
    sf::Vector2f position;
    sf::Clock bubbleMayhemLifetimeClock;
    float bubbleLifetime;

    BubbleMayhem(sf::Vector2f position, float lifetimeSec = 5.0f) : position(position), bubbleLifetime(lifetimeSec)
    {
        float randomBubbleSize = static_cast<float>((rand() % 41) + 50);
		sf::Vector2f size(randomBubbleSize, randomBubbleSize);

        hitbox.setSize(size);
        hitbox.setPosition(position);
        hitbox.setFillColor(sf::Color::Cyan);
    }

    bool isTimeExpired() const
    {
        return bubbleMayhemLifetimeClock.getElapsedTime().asSeconds() >= bubbleLifetime;
    }
};