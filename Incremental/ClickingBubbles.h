#pragma once

#include "Includes.h"

// Handles clicking bubbles
void clickHandler(long double& bubbles, long double &allTimeBubbles, long double &allTimeBubblesPerClick, long double baseBubblesPerClick, long double bubblesPerSecond, long double clickMultiplier = 1.0L)
{
    long double clickValue = (baseBubblesPerClick + (bubblesPerSecond * 0.05)) * clickMultiplier;

    bubbles += clickValue;
    allTimeBubbles += clickValue;
    allTimeBubblesPerClick += clickValue;

    cout << "Click registered! Current bubbles: " << bubbles << endl;
}