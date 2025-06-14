#pragma once

#include "Includes.h"

// Function to save the game state to a file
void saveFile(
    time_t timestamp,

    long double duckCounter,

    long double bubbles,
    long double allTimeBubbles,
	long double allTimeBubblesPerClick,

    long double baseBubblesPerClick,
    long double clickMultiplier,

    long double bubblesPerSecond,

    int soapCount,
    int handWashCount,
    int shampooCount
)
{
    ofstream saveFile("save_file.txt");

    if (saveFile.is_open())
    {
        saveFile << fixed << setprecision(0) << timestamp << endl;

        saveFile << fixed << setprecision(0) << duckCounter << endl;

        saveFile << fixed << setprecision(0) << bubbles << endl;
        saveFile << fixed << setprecision(2) << allTimeBubbles << endl;
		saveFile << fixed << setprecision(2) << allTimeBubblesPerClick << endl;

        saveFile << fixed << setprecision(2) << baseBubblesPerClick << endl;
        saveFile << fixed << setprecision(2) << clickMultiplier << endl;

        saveFile << fixed << setprecision(2) << bubblesPerSecond << endl;

        saveFile << fixed << setprecision(0) << soapCount << endl;
        saveFile << fixed << setprecision(0) << handWashCount << endl;
        saveFile << fixed << setprecision(0) << shampooCount << endl;

        saveFile.close();
        cout << "Game saved successfully." << endl;
    }

    else
    {
        cerr << "Unable to open save file." << endl;
    }
}

// Function to load the game state from a file
void loadFile(
    time_t &timestamp,

    long double &duckCounter,

    long double &bubbles,
    long double &allTimeBubbles,
	long double& allTimeBubblesPerClick,

    long double &baseBubblesPerClick,
    long double &clickMultiplier,

    long double &bubblesPerSecond,
    
    int &soapCount,
    int &handWashCount,
    int &shampooCount
)
{
    ifstream saveFile("save_file.txt");

    if (saveFile.is_open())
    {
        saveFile >> timestamp;

        saveFile >> duckCounter;

        saveFile >> bubbles;
        saveFile >> allTimeBubbles;
		saveFile >> allTimeBubblesPerClick;

        saveFile >> baseBubblesPerClick;
        saveFile >> clickMultiplier;

        saveFile >> bubblesPerSecond;

        saveFile >> soapCount;
        saveFile >> handWashCount;
        saveFile >> shampooCount;

        saveFile.close();
        cout << "Game loaded successfully." << endl;
    }

    else
    {
        cerr << "Unable to open save file. Starting a new game." << endl;
        timestamp = time(nullptr);

        duckCounter = 0.0f;

        bubbles = 0;
        allTimeBubbles = 0;
		allTimeBubblesPerClick = 0.0f;

        baseBubblesPerClick = 1.0f;
        clickMultiplier = 1.0f;

        bubblesPerSecond = 0.0f;

        soapCount = 0;
        handWashCount = 0;
        shampooCount = 0;
    }
}