#pragma once

#include "Includes.h"
#include "BuffTypes.h"

enum class duckVariantType
{
	Common, Uncommon, Rare, Epic,
	Legendary, Mythical, Divine, Celestial,
	Supreme, Cosmic, Astral, Ethereal,
	Omnipotent, Transcendent, Infinite, Rocket, Eternal // Thank you QQ for Rocket


	// Immortal, Godlike, Supreme, Ultimate,
	// Ascendant, Apex, Pinnacle, Zenith
	// Additional variants if needed/wanted
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
		sf::Color::Blue,
		59.9f,
		buffVariantType::rubberDuckBuff,
		duckVariantType::Common
	},
	{
		1.0f, 0.0f,
		sf::Color::Transparent,
		20.0f,
		buffVariantType::rubberDuckBuff,
		duckVariantType::Uncommon
	},
	{
		2.0f, 30.0f,
		sf::Color::Transparent,
		10.0f,
		buffVariantType::rubberDuckBuff,
		duckVariantType::Rare
	},
	{
		3.0f, 20.0f,
		sf::Color::Transparent,
		5.0f,
		buffVariantType::rubberDuckBuff,
		duckVariantType::Epic
	},
	{
		0.0f, 0.0f,
		sf::Color::Transparent,
		2.5f,
		buffVariantType::rubberDuckBuff,
		duckVariantType::Legendary
	},
	{
		0.0f, 0.0f,
		sf::Color::Transparent,
		1.0f,
		buffVariantType::rubberDuckBuff,
		duckVariantType::Mythical
	},
	{
		0.0f, 0.0f,
		sf::Color::Transparent,
		0.5f,
		buffVariantType::rubberDuckBuff,
		duckVariantType::Divine
	},
	{
		0.0f, 0.0f,
		sf::Color::Transparent,
		0.4f,
		buffVariantType::rubberDuckBuff,
		duckVariantType::Celestial
	},
	{
		0.0f, 0.0f,
		sf::Color::Transparent,
		0.3f,
		buffVariantType::rubberDuckBuff,
		duckVariantType::Supreme
	},
	{
		0.0f, 0.0f,
		sf::Color::Transparent,
		0.2f,
		buffVariantType::rubberDuckBuff,
		duckVariantType::Cosmic
	},
	{
		0.0f, 0.0f,
		sf::Color::Transparent,
		0.1f,
		buffVariantType::rubberDuckBuff,
		duckVariantType::Astral
	},
	{
		0.0f, 0.0f,
		sf::Color::Transparent,
		0.05f,
		buffVariantType::rubberDuckBuff,
		duckVariantType::Ethereal
	},
	{
		0.0f, 0.0f,
		sf::Color::Transparent,
		0.025f,
		buffVariantType::rubberDuckBuff,
		duckVariantType::Omnipotent
	},
	{
		0.0f, 0.0f,
		sf::Color::Transparent,
		0.015f,
		buffVariantType::rubberDuckBuff,
		duckVariantType::Transcendent
	},
	{
		1234.0f, 360.0f,
		sf::Color::Transparent,
		0.005f,
		buffVariantType::rubberDuckBuff,
		duckVariantType::Infinite
	},
	{
		9280.0f, 70.0f,
		sf::Color::Transparent,
		0.003f,
		buffVariantType::rubberDuckBuff,
		duckVariantType::Rocket
	},
	{
		77777.0f, 10.0f,
		sf::Color::Transparent,
		0.002f,
		buffVariantType::rubberDuckBuff,
		duckVariantType::Eternal
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