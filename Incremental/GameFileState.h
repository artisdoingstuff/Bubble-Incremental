#pragma once

#include "Includes.h"
#include "ObjectUpgrades.h"

// Function to save the game state to a file
void saveFileToJson(
    time_t timestamp,
    long double duckCounter,
    long double bubbles,
    long double allTimeBubbles,
    long double allTimeBubblesPerClick,
    long double baseBubblesPerClick,
    long double clickMultiplier,
    long double bubblesPerSecond,
    const std::vector<UpgradeItem>& upgrades
)
{
    json saveData;

    auto round2 = [](long double val) -> long double {
        return round(val * 100.0) / 100.0;
        };

    saveData["timestamp"] = timestamp;
    saveData["duckCounter"] = round2(duckCounter);
    saveData["bubbles"] = round2(bubbles);
    saveData["allTimeBubbles"] = round2(allTimeBubbles);
    saveData["allTimeBubblesPerClick"] = round2(allTimeBubblesPerClick);
    saveData["baseBubblesPerClick"] = round2(baseBubblesPerClick);
    saveData["clickMultiplier"] = round2(clickMultiplier);
    saveData["bubblesPerSecond"] = round2(bubblesPerSecond);
    saveData["upgrades"] = upgrades;  // Uses to_json from UpgradeItem

    ofstream file("save_file.json");
    if (file.is_open())
    {
        file << std::setw(4) << saveData << endl;
        file.close();
        cout << "Game saved successfully." << endl;
    }
    else
    {
        cerr << "Unable to open save_file.json for writing." << endl;
    }
}

// Function to load the game state from a file
void loadFileFromJson(
    time_t& timestamp,
    long double& duckCounter,
    long double& bubbles,
    long double& allTimeBubbles,
    long double& allTimeBubblesPerClick,
    long double& baseBubblesPerClick,
    long double& clickMultiplier,
    long double& bubblesPerSecond,
    vector<UpgradeItem>& upgrades
)
{
    ifstream file("save_file.json");
    if (!file.is_open())
    {
        cerr << "No save file found. Starting new game." << endl;
        timestamp = time(nullptr);
        return;
    }

    json saveData;
    file >> saveData;

    timestamp = saveData["timestamp"];
    duckCounter = saveData["duckCounter"];
    bubbles = saveData["bubbles"];
    allTimeBubbles = saveData["allTimeBubbles"];
    allTimeBubblesPerClick = saveData["allTimeBubblesPerClick"];
    baseBubblesPerClick = saveData["baseBubblesPerClick"];
    clickMultiplier = saveData["clickMultiplier"];
    bubblesPerSecond = saveData["bubblesPerSecond"];

    upgrades = saveData["upgrades"].get<vector<UpgradeItem>>();
    file.close();

    cout << "Game loaded from save_file.json" << endl;
}