#pragma once

#include "Includes.h"
#include "BuffTypes.h"

enum class goldenBubbleVariantType
{
	Multiplicative, Additive, BubbleChaos, BubbleFrenzy, BubbleMayhem
};

extern sf::Texture goldenBubbleTexture;

struct goldenBubbleBuffVariant
{
	float goldenBubbleMultiplier;
	float goldenBubbleDuration;
	sf::Color color;
	float goldenBubbleRarity;
	buffVariantType buffType = buffVariantType::goldenBubbleBuff;
	goldenBubbleVariantType goldenBubbleType = goldenBubbleVariantType::Multiplicative;
	sf::Sprite goldenBubbleSprite;

	goldenBubbleBuffVariant()
		: goldenBubbleSprite(goldenBubbleTexture)
	{}

	goldenBubbleBuffVariant(float buffMultiplier, float buffDuration, sf::Color color, float rarity,
		buffVariantType buffType, goldenBubbleVariantType goldenBubbleType)
		: goldenBubbleMultiplier(buffMultiplier), goldenBubbleDuration(buffDuration),
		color(color), goldenBubbleRarity(rarity), buffType(buffType), goldenBubbleType(goldenBubbleType),
		goldenBubbleSprite(goldenBubbleTexture)
	{}
};

inline vector<goldenBubbleBuffVariant> goldenBubbleVariants = {
	{
		5.0f, 10.0f,
		sf::Color::Blue,
		0.0f,
		buffVariantType::goldenBubbleBuff,
		goldenBubbleVariantType::Multiplicative
	},
	{
		1.0f, 0.0f,
		sf::Color::Blue,
		0.0f,
		buffVariantType::goldenBubbleBuff,
		goldenBubbleVariantType::Additive
	},
	{
		1.0, 20.0f,
		sf::Color::Blue,
		100.0f,
		buffVariantType::goldenBubbleBuff,
		goldenBubbleVariantType::BubbleChaos
	},
	{
		1.0, 20.0f,
		sf::Color::Blue,
		0.0f,
		buffVariantType::goldenBubbleBuff,
		goldenBubbleVariantType::BubbleFrenzy
	},
	{
		1.0, 20.0f,
		sf::Color::Blue,
		0.0f,
		buffVariantType::goldenBubbleBuff,
		goldenBubbleVariantType::BubbleMayhem
	}
};

inline void selectGoldenBubbleVariant(
	goldenBubbleBuffVariant& outVariant,
	sf::RectangleShape& hitbox,
	float& goldenBubbleMultiplier,
	float& goldenBubbleDuration
)
{
	float weightedRarity = 0.0f;
	for (const auto& variant : goldenBubbleVariants)
	{
		weightedRarity += variant.goldenBubbleRarity;
	}

	float randomValue = static_cast<float>(rand()) / RAND_MAX * weightedRarity;
	float cumulativeRarity = 0.0f;

	for (const auto& variant : goldenBubbleVariants)
	{
		cumulativeRarity += variant.goldenBubbleRarity;

		goldenBubbleBuffVariant currentVariant = variant;

		if (randomValue <= cumulativeRarity)
		{
			currentVariant.buffType = buffVariantType::goldenBubbleBuff;

			goldenBubbleMultiplier = currentVariant.goldenBubbleMultiplier;
			goldenBubbleDuration = currentVariant.goldenBubbleDuration;

			hitbox.setFillColor(currentVariant.color);

			currentVariant.goldenBubbleSprite.setTexture(goldenBubbleTexture);
			currentVariant.goldenBubbleSprite.setPosition(hitbox.getPosition());

			sf::Vector2f hitboxSize = hitbox.getSize();
			sf::Vector2u textureSize = goldenBubbleTexture.getSize();

			currentVariant.goldenBubbleSprite.setScale(sf::Vector2f(hitboxSize.x / textureSize.x, hitboxSize.y / textureSize.y));

			outVariant = currentVariant;
			outVariant.goldenBubbleSprite.setTexture(goldenBubbleTexture, true);

			return;
		}
	}

	outVariant = goldenBubbleVariants.front();
	outVariant.goldenBubbleSprite.setTexture(goldenBubbleTexture, true);
	outVariant.goldenBubbleSprite.setPosition(hitbox.getPosition());

	sf::Vector2f hitboxSize = hitbox.getSize();
	sf::Vector2u textureSize = goldenBubbleTexture.getSize();

	if (textureSize.x > 0 && textureSize.y > 0)
	{
		outVariant.goldenBubbleSprite.setScale({
			hitboxSize.x / static_cast<float>(textureSize.x),
			hitboxSize.y / static_cast<float>(textureSize.y)
			});
	}

	outVariant.goldenBubbleSprite.setColor(sf::Color(255, 255, 255, 0));

	return;
}