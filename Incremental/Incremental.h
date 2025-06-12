#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Window.hpp>

#include <SFML/System/Clock.hpp>

#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <utility>

using namespace std;

// Function to save the game state to a file
void saveFile(time_t timestamp, long double bubbles, long double allTimeBubbles, long double baseBubblesPerClick, long double bubblesPerSecond, int soapCount, int handWashCount, int shampooCount)
{
    ofstream saveFile("save_file.txt");

    if (saveFile.is_open())
    {
        saveFile << bubbles << endl;
        saveFile << allTimeBubbles << endl;
        saveFile << baseBubblesPerClick << endl;
        saveFile << bubblesPerSecond << endl;

        saveFile << soapCount << endl;
        saveFile << handWashCount << endl;
        saveFile << shampooCount << endl;

        saveFile << timestamp << endl;

        saveFile.close();
        cout << "Game saved successfully." << endl;
    }

    else
    {
        cerr << "Unable to open save file." << endl;
    }
}

// Function to load the game state from a file
void loadFile(time_t &timestamp, long double& bubbles, long double& allTimeBubbles, long double& baseBubblesPerClick, long double& bubblesPerSecond, int& soapCount, int& handWashCount, int& shampooCount)
{
    ifstream saveFile("save_file.txt");

    if (saveFile.is_open())
    {
        saveFile >> bubbles;
        saveFile >> allTimeBubbles;
        saveFile >> baseBubblesPerClick;
        saveFile >> bubblesPerSecond;

        saveFile >> soapCount;
        saveFile >> handWashCount;
        saveFile >> shampooCount;

        saveFile >> timestamp;

        saveFile.close();
        cout << "Game loaded successfully." << endl;
    }

    else
    {
        cerr << "Unable to open save file. Starting a new game." << endl;
        bubbles = 0;
        allTimeBubbles = 0;
        baseBubblesPerClick = 1.0f;
        bubblesPerSecond = 0.0f;

        soapCount = 0;
        handWashCount = 0;
        shampooCount = 0;

        timestamp = time(nullptr);
    }
}

// Offline earnings
void offlineBubbles(time_t timestamp, long double& bubbles, long double& allTimeBubbles, long double bubblesPerSecond)
{
    time_t currentTime = time(nullptr);
    time_t elapsedTime = currentTime - timestamp;

    if (elapsedTime > 0)
    {
        long double offlineBubbles = (elapsedTime * bubblesPerSecond) * 0.5;
        bubbles += offlineBubbles;
        allTimeBubbles += offlineBubbles;
        cout << "Offline bubbles: " << offlineBubbles << " bubbles" << endl;
    }
}

// Current cost of an object (building)
void currentObjectCost(long double& currentCost, long double baseCost, int objectCount, long double shopInflationMultiplier)
{
    currentCost = round(baseCost * pow(shopInflationMultiplier, objectCount));
}

// Upgrade logic so I don't spaghettify this file
void upgradeHandler(long double& bubbles, long double& bubblesPerSecond, long double& currentCost, long double baseCost, int& objectCount, long double addedBubblesPerSecond, long double shopInflationMultiplier)
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

long double clickHandler(long double& bubbles, long double baseBubblesPerClick, long double bubblesPerSecond)
{
    long double clickValue = baseBubblesPerClick + (bubblesPerSecond * 0.05);

    bubbles += clickValue;
    cout << "Click registered! Current bubbles: " << bubbles << endl;

    return clickValue;
}

string formatDisplayBubbles(long double bubbles)
{
    if (bubbles < 1000000.0f)
    {
        return to_string(static_cast<int>(bubbles));
    }

    const string bubblesSuffixes[] = {
        "", "", "M", "B", "T", "Qa", "Qi", "Sx", "Sp", "Oc", "No", "Dc",
        "Ud", "Dd", "Td", "Qad", "Qid", "Sxd", "Spd", "Ocd", "Nvd", "Vg",
        "Uvg", "Dvg", "Tvg", "Qavg", "Qivg", "Sxvg", "Spvg", "Ocvg", "Nvvg", "Tg",
        "Utg", "Dtg", "Ttg", "Qattg", "Qitg", "Sxtg", "Sptg", "Octg", "Nvtg", "Qg"
    };

    int logBubbles = log10(bubbles);
    int suffixGroup = logBubbles / 3;

    if (suffixGroup >= static_cast<int>(sizeof(bubblesSuffixes) / sizeof(bubblesSuffixes[0])))
        return "∞";

    int mod = logBubbles % 3;
    float mantissa = (bubbles / pow(10, logBubbles)) * pow(10, mod);

    ostringstream displayBubblesStringStream;
    displayBubblesStringStream << fixed << setprecision(2) << mantissa << bubblesSuffixes[suffixGroup];
    return displayBubblesStringStream.str();
}