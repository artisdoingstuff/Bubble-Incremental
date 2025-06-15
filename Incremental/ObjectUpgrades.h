#pragma once

#include "Includes.h"

extern const long double shopInflationMultiplier;

struct UpgradeItem
{
    string name;
    int count = 0;

    long double baseCost;
    long double currentCost;

    long double baseProduction; // bubbles per second
    long double unlockThreshold;

    bool isMilestone = false;
    bool unlockedByMilestone = false;
    long double milestoneTriggerValue = 0.0;

    void updateCost(const long double inflation = shopInflationMultiplier)
    {
        currentCost = round(baseCost * pow(inflation, count));
    }

    long double getProduction() const
    {
        return count * baseProduction;
    }

    bool isUnlocked(long double currentBubbles) const
    {
        return !unlockedByMilestone || isMilestone || currentBubbles >= milestoneTriggerValue;
    }

    bool canAfford(long double currentBubbles) const
    {
        return currentBubbles >= currentCost;
    }

    void purchase(long double& currentBubbles, const long double inflation = 1.15L)
    {
        if (!canAfford(currentBubbles)) return;

        currentBubbles -= currentCost;
        count++;
        updateCost(inflation);
    }
};

inline void to_json(json& j, const UpgradeItem& u)
{
    j = json{
        {"name", u.name},
        {"count", u.count},
        {"baseCost", u.baseCost},
        {"baseProduction", u.baseProduction},
        {"unlockThreshold", u.unlockThreshold},
        {"isMilestone", u.isMilestone},
        {"unlockedByMilestone", u.unlockedByMilestone},
        {"milestoneTriggerValue", u.milestoneTriggerValue}
    };
}

inline void from_json(const json& j, UpgradeItem& u)
{
    j.at("name").get_to(u.name);
    j.at("count").get_to(u.count);
    j.at("baseCost").get_to(u.baseCost);
    j.at("baseProduction").get_to(u.baseProduction);
    j.at("unlockThreshold").get_to(u.unlockThreshold);
    if (j.contains("isMilestone")) j.at("isMilestone").get_to(u.isMilestone);
    if (j.contains("unlockedByMilestone")) j.at("unlockedByMilestone").get_to(u.unlockedByMilestone);
    if (j.contains("milestoneTriggerValue")) j.at("milestoneTriggerValue").get_to(u.milestoneTriggerValue);

    u.updateCost();
}