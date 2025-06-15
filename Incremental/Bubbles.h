#pragma once

#include "Includes.h"

inline void addBubbles(
    long double amount,
    long double& bubbles,
    long double& allTimeBubbles,
    long double* allTimeBubblesPerClick = nullptr,
    bool isClick = false
)
{
    bubbles += amount;
    allTimeBubbles += amount;

    if (isClick)
        *allTimeBubblesPerClick += amount;
}