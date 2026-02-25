#pragma once

#include "../Misc/Globals/GIncludes.hpp"
#include "../Hardware/Logic.hpp"
#include "../Hardware/Hotfixes.hpp"

inline void reinitialise() {
    if (!kernelTree[2].overwritten) {
        for (auto& lg : logicGateList) {
            lg.ver = 0;
            lg.currentCost = lg.baseCost * costMult;
        }
    }
    for (auto& hf : hotfixList) hf.written = 0;

    bits = kernelTree[4].overwritten ? 1e9L : 0.0L;
    bitsPerSecond = 0.0L;
    hotfixMult = 1.0L;
}

inline long double getPendingBytes(long double bits) {
    if (bits < 5000000.0L) return 0.0L;
    return std::round(((std::pow(bits / 5000000.0L, 0.85L) * (5000000.0L * bitsToBytesRate)) * byteMultiplier) * 100.0L) / 100.0L;
}

inline void drawReinitialisationPopup(sf::RenderWindow& window, bool& startInit, sf::Vector2f& centre) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    sf::Vector2f boxSize(550.f, 220.f);

    long double pending = getPendingBytes(bits);

    auto triggerInit = [&]() {
        startInit = true;
        bytes += pending;
        allBytes += pending;
        reinitialise();
		timesInitialised++;
        activeTab = Tab::NONE;
        showReinitialisationPopup = false;
    };

    auto cancelInit = [&]() {
        activeTab = Tab::NONE;
        showReinitialisationPopup = false;
    };

    drawTabBox(window, mousePos, boxSize,
        "WARNING: System Re-initialisation Requested.\n"
        "All current data will be wiped.\n"
        "Unforeseen consequences possible.\n"
        "You will gain -" + format(getPendingBytes(bits), true) + " Bytes.\n\n"
        "Proceed with operation? (Y/N) > ", triggerInit, cancelInit);
}