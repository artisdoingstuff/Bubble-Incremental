#pragma once

#include "Includes.h"
#include "BuffTypes.h"

enum class goldenBubbleVariantType
{
    Normal, Multiplicative, Additive,
    BubbleChaos, BubbleFrenzy, BubbleMayhem
};

struct goldenBubbleBuffVariant
{
    float goldenBubbleMultiplier;
    float goldenBubbleDuration;
    sf::Color color;
    float goldenBubbleRarity;
    buffVariantType buffType = buffVariantType::goldenBubbleBuff;
    goldenBubbleVariantType goldenBubbleType = goldenBubbleVariantType::Multiplicative;
    sf::Texture* texturePtr = nullptr;
};

// Texture references
extern sf::Texture bubbleTexture;
extern sf::Texture goldenBubbleTexture;

inline std::vector<goldenBubbleBuffVariant> goldenBubbleVariants = {
    {
        2.0f, 30.0f,
        sf::Color::Blue,
        60.0f,
        buffVariantType::goldenBubbleBuff,
        goldenBubbleVariantType::Normal,
        &bubbleTexture
    },
    {
        3.0f, 15.0f,
        sf::Color::Blue,
        10.0f,
        buffVariantType::goldenBubbleBuff,
        goldenBubbleVariantType::Multiplicative,
        &goldenBubbleTexture
    },
    {
        1.0f, 0.0f, 
        sf::Color::Blue,
        20.0f,
        buffVariantType::goldenBubbleBuff,
        goldenBubbleVariantType::Additive,
        & goldenBubbleTexture
    },
    {
        0.4f, 20.0f,
        sf::Color::Blue,
        1.0f,
        buffVariantType::goldenBubbleBuff,
        goldenBubbleVariantType::BubbleChaos,
        & goldenBubbleTexture
    },
    {
        3.0f, 30.0f,
        sf::Color::Blue,
        6.0f,
        buffVariantType::goldenBubbleBuff,
        goldenBubbleVariantType::BubbleFrenzy,
        & goldenBubbleTexture
    },
    {
        1.5f, 20.0f,
        sf::Color::Blue,
        3.0f,
        buffVariantType::goldenBubbleBuff,
        goldenBubbleVariantType::BubbleMayhem,
        & goldenBubbleTexture
    }
};

inline void selectGoldenBubbleVariant(
    goldenBubbleBuffVariant& outVariant,
    sf::RectangleShape& hitbox,
    float& goldenBubbleMultiplier,
    float& goldenBubbleDuration,
    sf::Sprite& outSprite
)
{
    float totalRarity = 0.0f;
    for (const auto& variant : goldenBubbleVariants)
        totalRarity += variant.goldenBubbleRarity;

    float roll = static_cast<float>(rand()) / RAND_MAX * totalRarity;
    float cumulative = 0.0f;

    for (const auto& variant : goldenBubbleVariants)
    {
        cumulative += variant.goldenBubbleRarity;
        if (roll <= cumulative)
        {
            outVariant = variant;
            goldenBubbleMultiplier = variant.goldenBubbleMultiplier;
            goldenBubbleDuration = variant.goldenBubbleDuration;

            hitbox.setFillColor(variant.color);

            outSprite.setTexture(*variant.texturePtr, true);
            outSprite.setPosition(hitbox.getPosition());

            sf::Vector2f hitboxSize = hitbox.getSize();
            sf::Vector2u textureSize = variant.texturePtr->getSize();
            if (textureSize.x > 0 && textureSize.y > 0)
            {
                outSprite.setScale({
                    hitboxSize.x / static_cast<float>(textureSize.x),
                    hitboxSize.y / static_cast<float>(textureSize.y)
                    });
            }

            return;
        }
    }

    // Fallback
    outVariant = goldenBubbleVariants.front();
    outSprite.setTexture(*outVariant.texturePtr, true);
    outSprite.setPosition(hitbox.getPosition());

    sf::Vector2f hitboxSize = hitbox.getSize();
    sf::Vector2u textureSize = outVariant.texturePtr->getSize();
    if (textureSize.x > 0 && textureSize.y > 0)
    {
        outSprite.setScale({
            hitboxSize.x / static_cast<float>(textureSize.x),
            hitboxSize.y / static_cast<float>(textureSize.y)
            });
    }
}