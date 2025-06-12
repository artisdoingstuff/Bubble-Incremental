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
void saveGame(long double currency, long double allTimeCurrency, long double baseCurrencyPerClick, long double currencyPerSecond, int soapCount, int handWashCount, int shampooCount)
{
    ofstream saveFile("save_file.txt");

    if (saveFile.is_open())
    {
        saveFile << currency << endl;
        saveFile << allTimeCurrency << endl;
        saveFile << baseCurrencyPerClick << endl;
        saveFile << currencyPerSecond << endl;

        saveFile << soapCount << endl;
        saveFile << handWashCount << endl;
        saveFile << shampooCount << endl;

        saveFile.close();
        cout << "Game saved successfully." << endl;
    }

    else
    {
        cerr << "Unable to open save file." << endl;
    }
}

// Function to load the game state from a file
void loadGame(long double& currency, long double& allTimeCurrency, long double& baseCurrencyPerClick, long double& currencyPerSecond, int& soapCount, int& handWashCount, int& shampooCount)
{
    ifstream saveFile("save_file.txt");

    if (saveFile.is_open())
    {
        saveFile >> currency;
        saveFile >> allTimeCurrency;
        saveFile >> baseCurrencyPerClick;
        saveFile >> currencyPerSecond;

        saveFile >> soapCount;
        saveFile >> handWashCount;
        saveFile >> shampooCount;

        saveFile.close();
        cout << "Game loaded successfully." << endl;
    }

    else
    {
        cerr << "Unable to open save file. Starting a new game." << endl;
        currency = 0;
        allTimeCurrency = 0;
        baseCurrencyPerClick = 1.0f;
        currencyPerSecond = 0.0f;

        soapCount = 0;
        handWashCount = 0;
        shampooCount = 0;
    }
}

// Current cost of an object (building)
void currentObjectCost(long double& currentCost, long double baseCost, int objectCount, long double shopInflationMultiplier)
{
    currentCost = round(baseCost * pow(shopInflationMultiplier, objectCount));
}

// Upgrade logic so I don't spaghettify this file
void upgradeHandler(long double& currency, long double& currencyPerSecond, long double& currentCost, long double baseCost, int& objectCount, long double addedCurrencyPerSecond, long double shopInflationMultiplier)
{
    currentObjectCost(currentCost, baseCost, objectCount, shopInflationMultiplier);

    currency -= currentCost;

    if (currency < 0)
    {
        currency += currentCost;
        cout << "Not enough currency to purchase upgrade!" << endl;
    }

    else
    {
        objectCount++;
        currencyPerSecond += addedCurrencyPerSecond;
        cout << "Upgrade purchased (" + to_string(currentCost) + ")! Current count: " << objectCount << endl;
    }
}

long double clickHandler(long double& currency, long double baseCurrencyPerClick, long double currencyPerSecond)
{
    long double clickValue = baseCurrencyPerClick + (currencyPerSecond * 0.05);

    currency += clickValue;
    cout << "Click registered! Current currency: " << currency << endl;

    return clickValue;
}