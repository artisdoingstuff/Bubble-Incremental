#pragma once

#include "Includes.h"

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