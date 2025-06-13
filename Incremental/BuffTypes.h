#pragma once

#include "Includes.h"

enum class buffVariantType
{
	none,
	bubbleBuff, goldenBubbleBuff,
	rubberDuckBuff // this is the duck
};

struct buffVariantBuffs
{
	float buffMultiplier = 1.0f;
	float buffDuration = 0.0f;
	sf::Color color;
	buffVariantType type;
};