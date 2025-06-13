#pragma once

#include "Includes.h"
#include "BuffTypes.h"

enum class duckVariantType
{
	Common, Uncommon, Rare, Epic,
	Legendary, Mythical, Divine, Celestial,
	Olympian, Cosmic, Astral, Ethereal,
	Omnipotent, Transcendent, Infinite, Eternal
};

struct duckBuffVariant
{
	float duckMultiplier;
	float duckDuration;
	sf::Color color;
	float duckRarity;
	buffVariantType buffType = buffVariantType::rubberDuckBuff;
	duckVariantType duckType = duckVariantType::Common;
};

inline vector<duckBuffVariant> duckVariants = {
	{
		1.0f, 0.0f,
		sf::Color::Red,
		70.0f,
		buffVariantType::rubberDuckBuff,
		duckVariantType::Common
	},
	{
		1.5f, 5.0f,
		sf::Color::Green,
		25.0f,
		buffVariantType::rubberDuckBuff,
		duckVariantType::Uncommon
	},
	{
		2.0f, 10.0f,
		sf::Color::Cyan,
		5.0f,
		buffVariantType::rubberDuckBuff,
		duckVariantType::Rare
	}
};

inline duckBuffVariant selectDuckVariant(sf::RectangleShape& hitbox, float& duckMultiplier, float& duckDuration)
{
	float weightedRarity = 0.0f;
	for (const auto& variant : duckVariants)
	{
		weightedRarity += variant.duckRarity;
	}

	float randomValue = static_cast<float>(rand()) / RAND_MAX * weightedRarity;
	float cumulativeRarity = 0.0f;

	for (const auto& variant : duckVariants)
	{
		cumulativeRarity += variant.duckRarity;
		if (randomValue <= cumulativeRarity)
		{
			duckMultiplier = variant.duckMultiplier;
			duckDuration = variant.duckDuration;
			hitbox.setFillColor(variant.color);
			return variant;
		}
	}

	return duckVariants.front();
}