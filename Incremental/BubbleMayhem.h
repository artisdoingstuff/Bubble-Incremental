#include "Includes.h"

extern sf::Texture bubbleTexture;

struct BubbleMayhem
{
    sf::RectangleShape hitbox;

    sf::Sprite bubbleSprite;

    sf::Vector2f position;
    sf::Clock bubbleMayhemLifetimeClock;
    float bubbleLifetime;

    BubbleMayhem(sf::Vector2f position, float lifetimeSec = 5.0f)
        : position(position), bubbleLifetime(lifetimeSec), bubbleSprite(bubbleTexture)
    {
        float randomBubbleSize = static_cast<float>((rand() % 41) + 50);
		sf::Vector2f size(randomBubbleSize, randomBubbleSize);

		bubbleSprite.setTexture(bubbleTexture);
        bubbleSprite.setPosition(position);
		bubbleSprite.setScale(sf::Vector2f(size.x / bubbleTexture.getSize().x, size.y / bubbleTexture.getSize().y));

        hitbox.setSize(size);
        hitbox.setPosition(position);
        hitbox.setFillColor(sf::Color::Transparent);
    }

    bool isTimeExpired() const
    {
        return bubbleMayhemLifetimeClock.getElapsedTime().asSeconds() >= bubbleLifetime;
    }

    void updateBubbleAlpha()
    {
        float elapsed = bubbleMayhemLifetimeClock.getElapsedTime().asSeconds();

        float fadeInTime = 0.5f;
        float fadeOutTime = 0.5f;

        float alpha = 255.0f;

        if (elapsed < fadeInTime)
        {
            alpha = (elapsed / fadeInTime) * 255.0f;
        }
        else if (elapsed > bubbleLifetime - fadeOutTime)
        {
            float remaining = bubbleLifetime - elapsed;
            alpha = (remaining / fadeOutTime) * 255.0f;
        }

        if (alpha < 0.0f) alpha = 0.0f;
        if (alpha > 255.0f) alpha = 255.0f;

        sf::Color color = bubbleSprite.getColor();
        color.a = static_cast<int>(alpha);
        bubbleSprite.setColor(color);
    }
};