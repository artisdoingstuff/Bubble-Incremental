#pragma once

#include "Includes.h"

struct Achievement
{
    string name;
    string description;
    bool unlocked = false;
    long double triggerValue; // Threshold

    enum class Type { Bubbles, Clicks, Upgrades } type;

    Achievement(const string& achievement, const string& description, Type t, long double trigger)
        : name(achievement), description(description), type(t), triggerValue(trigger) {
    }
};

vector<Achievement> achievements;

inline void checkAchievements(
    const long double& allTimeBubbles,
    const long double& allTimeBubblesPerClick,
    const long double& totalUpgradeCount,
    const sf::Font& font
)
{
    for (auto& a : achievements)
    {
        if (a.unlocked)
            continue;

        bool shouldUnlock = false;

        switch (a.type)
        {
        case Achievement::Type::Bubbles:
            shouldUnlock = allTimeBubbles >= a.triggerValue;
            break;
        case Achievement::Type::Clicks:
            shouldUnlock = allTimeBubblesPerClick >= a.triggerValue;
            break;
        case Achievement::Type::Upgrades:
            shouldUnlock = totalUpgradeCount >= a.triggerValue;
            break;
        }

        if (shouldUnlock)
        {
            a.unlocked = true;

            cout << "[ACHIEVEMENT] " << a.name << ": " << a.description << endl;
        }
    }
}

inline void to_json(json& j, const Achievement& a)
{
    j = json{
        {"name", a.name},
        {"description", a.description},
        {"triggerValue", a.triggerValue},
        {"type", static_cast<int>(a.type)},
        {"unlocked", a.unlocked}
    };
}

inline void from_json(const json& j, Achievement& a)
{
    j.at("name").get_to(a.name);
    j.at("description").get_to(a.description);
    j.at("triggerValue").get_to(a.triggerValue);

    int typeInt;
    j.at("type").get_to(typeInt);
    a.type = static_cast<Achievement::Type>(typeInt);

    j.at("unlocked").get_to(a.unlocked);
}