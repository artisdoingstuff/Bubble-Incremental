#pragma once

#include "Includes.h"
#include "BuffTypes.h"
#include "DuckVariants.h"
#include "GlobalBubblesVariants.h"

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

    if (!showBuffHitbox && buffSpawnIntervalClock.getElapsedTime().asSeconds() >= buffSpawnInterval)
    {
        float x = static_cast<float>(rand() % 600) + 550.f;
        float y = static_cast<float>(rand() % 800) + 50.f;

        buffHitbox.setPosition({ x, y });
        showBuffHitbox = true;
        buffLifetimeClock.restart();
        buffSpawnIntervalClock.restart();

        buffSpawnInterval = static_cast<float>(
            rand() % static_cast<int>(maxSpawnInterval - minSpawnInterval + 1)
            + static_cast<int>(minSpawnInterval)
            );

        if (isVariant && variantSelector)
            variantSelector(buffHitbox, buffMultiplier, buffDuration);

        if (useSprite && buffSprite)
        {
            buffSprite->setPosition(buffHitbox.getPosition());
            sf::Color spriteColor = buffSprite->getColor();
            spriteColor.a = 0;
            buffSprite->setColor(spriteColor);
        }

        sf::Color c = buffHitbox.getFillColor();
        c.a = 0;
        buffHitbox.setFillColor(c);
    }

    if (showBuffHitbox && !isBuffActive && elapsedBuffLifetime <= 1.0f)
    {
        int alpha = static_cast<int>(min(elapsedBuffLifetime / 1.0f * 255.0f, 255.0f));
        sf::Color color = buffHitbox.getFillColor();
        color.a = alpha;
        buffHitbox.setFillColor(color);

        if (useSprite && buffSprite)
        {
            sf::Color sc = buffSprite->getColor();
            sc.a = alpha;
            buffSprite->setColor(sc);
        }
    }

    if (showBuffHitbox && !isBuffActive && elapsedBuffLifetime > 180.0f)
    {
        int alpha = static_cast<int>(max(0.0f, (1.0f - (elapsedBuffLifetime - 180.0f)) * 255.0f));
        sf::Color color = buffHitbox.getFillColor();
        color.a = alpha;
        buffHitbox.setFillColor(color);

        if (useSprite && buffSprite)
        {
            sf::Color sc = buffSprite->getColor();
            sc.a = alpha;
            buffSprite->setColor(sc);
        }

        if (elapsedBuffLifetime >= 10.0f)
        {
            showBuffHitbox = false;
            buffHitbox.setPosition({ -100.f, -100.f });
            buffHitbox.setFillColor(sf::Color(0, 0, 255, 255));

            if (useSprite && buffSprite)
                buffSprite->setPosition({ -100.f, -100.f });
        }
    }

    if (showBuffHitbox && !isBuffActive && elapsedBuffLifetime > 182.0f)
    {
        cout << "[Failsafe] Resetting stuck buff at " << buffHitbox.getPosition().x << ", " << buffHitbox.getPosition().y << endl;
        showBuffHitbox = false;
        buffHitbox.setPosition({ -100.f, -100.f });

        if (useSprite && buffSprite)
            buffSprite->setPosition({ -100.f, -100.f });

        buffSpawnInterval = static_cast<float>(rand() % static_cast<int>(maxSpawnInterval - minSpawnInterval + 1) + minSpawnInterval);
        buffSpawnIntervalClock.restart();
        buffLifetimeClock.restart();
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
            onBuffClick();

        buffHitbox.setPosition({ -100.f, -100.f });

        if (useSprite && buffSprite)
            buffSprite->setPosition({ -100.f, -100.f });

        return true;
    }

    if (isBuffActive && buffDurationClock.getElapsedTime().asSeconds() > buffDuration)
    {
        isBuffActive = false;
        buffLifetimeClock.restart();
    }

    return false;
}