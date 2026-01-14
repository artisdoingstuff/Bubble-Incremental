#pragma once

#include "../Misc/GIncludes.hpp"

inline void drawLoadingUI(sf::RenderWindow& window, float actualProgress) {
    float width = 600.f;
    float height = 5.f;
    sf::Vector2f centre = { window.getSize().x / 2.f, window.getSize().y / 2.f };

    float time = std::clock() / static_cast<float>(CLOCKS_PER_SEC);
    float jitter = std::sin(time * 2.f) * 0.05f + std::sin(time * 5.f) * 0.02f;

    float visualProgress = actualProgress + jitter;
    if (visualProgress > actualProgress) visualProgress = actualProgress;
    if (visualProgress < 0.f) visualProgress = 0.f;

    if ((actualProgress > 0.35f && actualProgress < 0.45f) || (actualProgress > 0.85f && actualProgress < 0.98f)) {
        visualProgress -= 0.05f;
    }

    static std::string currentStatus = "Initializing...";
    static float lastUpdate = 0.f;

    std::vector<std::string> messages = {
        "Purging Bits...", "Realigning Star...", "Cleaning Cache...",
        "Defragmenting Void...", "Recalculating Reality...", "Patching Void.INC.exe...",
        "Formatting Logic...", "Flushing Memory...", "Bypassing Hotfixes...", "Destroying V-1D..."
    };

    if (actualProgress < 0.85f) {
        if (!reinitialisation) currentStatus = "Closing Root.dir...";
        else {
            if (time - lastUpdate > 2.f) {
                currentStatus = messages[std::rand() % messages.size()];
                lastUpdate = time;
            }
        }
    }
    else {
        if (reinitialisation) currentStatus = "Opening Root.dir...";
        if (initialisation) currentStatus = "Initialising...";
    }

    sf::RectangleShape bg({ width, height });
    bg.setOrigin({ width / 2.f, height / 2.f });
    bg.setPosition(centre);
    bg.setFillColor(sf::Color(30, 30, 30));
    window.draw(bg);

    sf::RectangleShape fill({ width * std::clamp(visualProgress, 0.f, 1.f), height });
    fill.setOrigin({ 0.f, height / 2.f });
    fill.setPosition(sf::Vector2f(centre.x - (width / 2.f), centre.y));
    fill.setFillColor(sf::Color(243, 238, 225));
    window.draw(fill);

    sf::Text pct(jetBrainsMono);
    pct.setString(std::to_string(static_cast<int>(std::clamp(visualProgress, 0.f, 1.f) * 100)) + "%");
    pct.setCharacterSize(16);
    pct.setFillColor(sf::Color(243, 238, 225));
    pct.setPosition(sf::Vector2f(centre.x - 15.f, centre.y + 20.f));
    window.draw(pct);

    sf::Text status(jetBrainsMono);
    status.setString(currentStatus);
    status.setCharacterSize(14);
    status.setFillColor(sf::Color(150, 150, 150));
    status.setPosition(sf::Vector2f(centre.x - (status.getGlobalBounds().size.x / 2.f), centre.y - 30.f));
    window.draw(status);
}