#pragma once

#include "Includes.h"

// Handles clicking bubbles
long double clickHandler(long double& bubbles, long double baseBubblesPerClick, long double bubblesPerSecond)
{
    long double clickValue = baseBubblesPerClick + (bubblesPerSecond * 0.05);

    bubbles += clickValue;
    cout << "Click registered! Current bubbles: " << bubbles << endl;

    return clickValue;
}