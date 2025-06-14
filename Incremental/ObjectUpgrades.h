#pragma once

#include "Includes.h"

// Current cost of an object (building)
void currentObjectCost(long double& currentCost, long double baseCost, int objectCount, long double shopInflationMultiplier)
{
    currentCost = round(baseCost * pow(shopInflationMultiplier, objectCount));
}

// Upgrade logic so I don't spaghettify this file
void objectUpgradeHandler(long double& bubbles, long double& bubblesPerSecond, long double& currentCost, long double baseCost, int& objectCount, long double addedBubblesPerSecond, long double shopInflationMultiplier)
{
    currentObjectCost(currentCost, baseCost, objectCount, shopInflationMultiplier);

    bubbles -= currentCost;

    if (bubbles < 0)
    {
        bubbles += currentCost;
        cout << "Not enough bubbles to purchase upgrade!" << endl;
    }

    else
    {
        objectCount++;
        bubblesPerSecond += addedBubblesPerSecond;
        cout << "Upgrade purchased (" + to_string(currentCost) + ")! Current count: " << objectCount << endl;
    }
}