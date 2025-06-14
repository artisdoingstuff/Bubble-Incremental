#pragma once

#include "Includes.h"
#include "BuffTypes.h"

enum class goldenBubbleVariantType
{
	Multiplicative, Additive, BubbleMayhem
};

struct goldenBubbleBuffVariant
{
	float goldenBubbleMultiplier;
	float goldenBubbleDuration;
	sf::Color color;
	float goldenBubbleRarity;
	buffVariantType buffType = buffVariantType::goldenBubbleBuff;
	goldenBubbleVariantType goldenBubbleType = goldenBubbleVariantType::Multiplicative;
};

inline vector<goldenBubbleBuffVariant> goldenBubbleVariants = {
	{
		5.0f, 10.0f,
		sf::Color::Red,
		0.0f,
		buffVariantType::goldenBubbleBuff,
		goldenBubbleVariantType::Multiplicative
	},
	{
		1.0f, 0.0f,
		sf::Color::Red,
		0.0f,
		buffVariantType::goldenBubbleBuff,
		goldenBubbleVariantType::Additive
	},
	{
		1.0, 20.0f,
		sf::Color::Yellow,
		100.0f,
		buffVariantType::goldenBubbleBuff,
		goldenBubbleVariantType::BubbleMayhem
	}
};

inline goldenBubbleBuffVariant selectgoldenBubbleVariant(sf::RectangleShape& hitbox, float& goldenBubbleMultiplier, float& goldenBubbleDuration)
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
		if (randomValue <= cumulativeRarity)
		{
			goldenBubbleMultiplier = variant.goldenBubbleMultiplier;
			goldenBubbleDuration = variant.goldenBubbleDuration;
			hitbox.setFillColor(variant.color);
			return variant;
		}
	}

	return goldenBubbleVariants.front();
}