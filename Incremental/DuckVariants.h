#pragma once

#include "Includes.h"

enum class duckVariantType
{
	Common, Uncommon, Rare, Epic,
	Legendary, Mythical, Divine, Celestial,
	Olympian, Cosmic, Astral, Ethereal,
	Omnipotent, Transcendent, Infinite, Eternal
};

struct duckVariant
{
	float duckMultiplier = 0.0f;
	float duckDuration = 0.0f;
	sf::Color color;
	float duckRarity;
	duckVariantType type;
	
};

inline vector<duckVariant> duckVariants = {
	{ 1.0f, 0.0f, sf::Color::Red, 70.0f, duckVariantType::Common},
	{ 1.5f, 5.0f, sf::Color::Green, 25.0f, duckVariantType::Uncommon},
	{ 2.0f, 10.0f, sf::Color::Cyan, 5.0f, duckVariantType::Rare}
};

inline duckVariant selectDuckVariant(
	float& duckMultiplier,
	float& duckDuration,
	sf::RectangleShape& hitbox
)
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