#pragma once

#include "Includes.h"

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
    function<void(float&, float&, sf::RectangleShape&)> variantSelector = nullptr
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
            variantSelector(buffMultiplier, buffDuration, buffHitbox);
        }

        sf::Color color = buffHitbox.getFillColor();
        color.a = 0;
        buffHitbox.setFillColor(color);
    }

    if (showBuffHitbox && !isBuffActive && elapsedBuffLifetime <= 1.0f)
    {
        float fadeInAlpha = elapsedBuffLifetime / 1.0f * 255.0f;
        fadeInAlpha = std::min(fadeInAlpha, 255.0f);

        sf::Color color = buffHitbox.getFillColor();
        color.a = static_cast<int>(fadeInAlpha);
        buffHitbox.setFillColor(color);
    }

    if (showBuffHitbox && !isBuffActive && elapsedBuffLifetime > 8.0f)
    {
        float fadeOutAlpha = (1.0f - (elapsedBuffLifetime - 8.0f)) * 255.0f;
        fadeOutAlpha = std::max(0.0f, min(fadeOutAlpha, 255.0f));

        sf::Color color = buffHitbox.getFillColor();
        color.a = static_cast<int>(fadeOutAlpha);
        buffHitbox.setFillColor(color);

        if (showBuffHitbox && elapsedBuffLifetime >= 10.0f)
        {
            showBuffHitbox = false;
            buffHitbox.setPosition({ -100.f, -100.f });
            buffHitbox.setFillColor(sf::Color(0, 0, 255, 255));
        }
    }

    if (isCurrentlyPressed && !isButtonPressed && buffHitbox.getGlobalBounds().contains(mousePos))
    {
        isBuffActive = true;
        buffDurationClock.restart();
        buffLifetimeClock.restart();
        showBuffHitbox = false;

        buffHitbox.setPosition({ -100, -100 });

        return true;
    }

    if (isBuffActive && buffDurationClock.getElapsedTime().asSeconds() > buffDuration)
    {
        isBuffActive = false;
        buffLifetimeClock.restart();
    }

    return false;
}