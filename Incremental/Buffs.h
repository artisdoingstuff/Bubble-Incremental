#pragma once

#include "Includes.h"
#include "BuffTypes.h"
#include "DuckVariants.h"
#include "GoldenBubblesVariants.h"

bool buffHandler(
    const sf::Vector2f& mousePos,
    sf::RenderWindow& window,

    sf::RectangleShape& buffHitbox,

    sf::Clock& buffDurationClock,
    sf::Clock& buffSpawnIntervalClock,
    sf::Clock& buffLifetimeClock,

    bool& isBuffActive,
    bool& showBuffHitbox,

    float& buffSpawnInterval,
    float& buffMultiplier,
    float& buffDuration,
    float minSpawnInterval,
    float maxSpawnInterval,

    bool isCurrentlyPressed,
    bool isButtonPressed,

    bool isVariant = false,
    function<void(sf::RectangleShape&, float&, float&)> variantSelector = nullptr,
    function<void()> onBuffClick = nullptr,

    sf::Sprite* buffSprite = nullptr,
    bool useSprite = false
)
{
    float elapsedBuffLifetime = buffLifetimeClock.getElapsedTime().asSeconds();

    if (!showBuffHitbox && buffSpawnIntervalClock.getElapsedTime().asSeconds() > buffSpawnInterval)
    {
        float x = static_cast<float>(rand() % static_cast<int>(600)) + 550;
        float y = static_cast<float>(rand() % static_cast<int>(800)) + 50;

        buffHitbox.setPosition({ x, y });

        showBuffHitbox = true;
        buffSpawnIntervalClock.restart();
        buffLifetimeClock.restart();
        buffSpawnInterval = static_cast<float>(rand() % static_cast<int>(maxSpawnInterval - minSpawnInterval + 1) + minSpawnInterval);

        if (isVariant && variantSelector)
        {
            variantSelector(
                buffHitbox,
                buffMultiplier,
                buffDuration
            );
        }

        if (useSprite && buffSprite)
        {
            buffSprite->setPosition(buffHitbox.getPosition());
            sf::Color spriteColor = buffSprite->getColor();
            spriteColor.a = 0;
            buffSprite->setColor(sf::Color(255, 255, 255, 0));
        }

        sf::Color color = buffHitbox.getFillColor();
        color.a = 0;
        buffHitbox.setFillColor(color);
    }

    if (showBuffHitbox && !isBuffActive && elapsedBuffLifetime <= 1.0f)
    {
        float fadeInAlpha = elapsedBuffLifetime / 1.0f * 255.0f;
        int alpha = static_cast<int>(min(fadeInAlpha, 255.0f));

        sf::Color color = buffHitbox.getFillColor();
        color.a = alpha;
        buffHitbox.setFillColor(color);

        if (useSprite && buffSprite)
        {
            sf::Color spriteColor = buffSprite->getColor();
            spriteColor.a = alpha;
            buffSprite->setColor(sf::Color(255, 255, 255, alpha));
        }
    }

    if (showBuffHitbox && !isBuffActive && elapsedBuffLifetime > 8.0f)
    {
        float fadeOutAlpha = (1.0f - (elapsedBuffLifetime - 8.0f)) * 255.0f;
        int alpha = static_cast<int>(max(0.0f, min(fadeOutAlpha, 255.0f)));

        sf::Color color = buffHitbox.getFillColor();
        color.a = alpha;
        buffHitbox.setFillColor(color);

        if (useSprite && buffSprite)
        {
            sf::Color spriteColor = buffSprite->getColor();
            spriteColor.a = alpha;
            buffSprite->setColor(spriteColor);
        }

        if (showBuffHitbox && elapsedBuffLifetime >= 10.0f)
        {
            showBuffHitbox = false;
            buffHitbox.setPosition({ -100.f, -100.f });
            buffHitbox.setFillColor(sf::Color(0, 0, 255, 255));

            if (useSprite && buffSprite)
            {
                buffSprite->setPosition({ -100.f, -100.f });
                sf::Color spriteColor = buffSprite->getColor();
                buffSprite->setColor(sf::Color(255, 255, 255, spriteColor.a));
            }
        }
    }

    if (useSprite && buffSprite && showBuffHitbox)
    {
        window.draw(*buffSprite);
    }

    if (isCurrentlyPressed && !isButtonPressed && buffHitbox.getGlobalBounds().contains(mousePos))
    {
        isBuffActive = true;
        buffDurationClock.restart();
        buffLifetimeClock.restart();
        showBuffHitbox = false;

        if (onBuffClick)
        {
            onBuffClick();
        }

        buffHitbox.setPosition({ -100, -100 });

        if (useSprite && buffSprite)
        {
            buffSprite->setPosition({ -100, -100 });
        }

        return true;
    }

    if (isBuffActive && buffDurationClock.getElapsedTime().asSeconds() > buffDuration)
    {
        isBuffActive = false;
        buffLifetimeClock.restart();
    }

    return false;
}