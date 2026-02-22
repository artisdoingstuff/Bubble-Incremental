#pragma once

#include "../Misc/Globals/GIncludes.hpp"
#include "../Hardware/Logic.hpp"
#include "../Hardware/Hotfixes.hpp"

inline void reinitialise() {
    for (auto& lg : logicGateList) {
        lg.ver = 0;
        lg.currentCost = lg.baseCost * costMult;
    }

    for (auto& hf : hotfixList) hf.written = 0;

    bits = 0.0L;
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

    sf::RectangleShape overlay(sf::Vector2f(window.getSize()));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);

    sf::RectangleShape box(boxSize);
    box.setOrigin(boxSize / 2.f);
    box.setPosition(centre);
    box.setFillColor(sf::Color(10, 10, 10));
    box.setOutlineColor(sf::Color(50, 50, 50));
    box.setOutlineThickness(1);

    sf::RectangleShape titleBar({ boxSize.x, 30.f });
    titleBar.setOrigin({ boxSize.x / 2.f, 0.f });
    titleBar.setPosition({ centre.x, centre.y - (boxSize.y / 2.f) });
    titleBar.setFillColor(sf::Color(40, 40, 40));

    sf::Text t(jetBrainsMono, "> void://hardware/reinit.bat" + getCursor(), 14);
    t.setPosition({ titleBar.getPosition().x - (boxSize.x / 2.f) + 10.f, titleBar.getPosition().y + 5.f });
    t.setFillColor(sf::Color(243, 238, 225));

    sf::Text c(jetBrainsMono, "- X", 14);
    c.setPosition({ titleBar.getPosition().x + ((boxSize.x / 2.f) * 0.85f), titleBar.getPosition().y + 5.f });
    c.setFillColor(sf::Color(243, 238, 225));

    window.draw(box);
    window.draw(titleBar);
    window.draw(t);
    window.draw(c);

    std::string warning =
        "WARNING: System Re-initialisation Requested.\n"
        "All current data will be wiped.\n"
        "Unforeseen consequences possible.\n"
		"You will gain -" + format(getPendingBytes(bits), true) + " Bytes.\n\n"
        "Proceed with operation? (Y/N) > " + getCursor();

    sf::Text w(jetBrainsMono, warning, 16);
    w.setOrigin({ w.getGlobalBounds().size.x / 2.f, w.getGlobalBounds().size.y / 2.f - 10.f });
    w.setPosition(centre);
    w.setFillColor(sf::Color(200, 200, 200));

    window.draw(w);

    auto triggerInit = [&]() {
        startInit = true;
        bytes += getPendingBytes(bits);
        allBytes += getPendingBytes(bits);
        reinitialise();
		timesInitialised++;
        activeTab = Tab::NONE;
        showReinitialisationPopup = false;
    };

    auto cancelInit = [&]() {
        activeTab = Tab::NONE;
        showReinitialisationPopup = false;
    };

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y)) {
        triggerInit(); playSFX("button");
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) || sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && c.getGlobalBounds().contains(mousePos)) {
        cancelInit(); playSFX("button");
    }
}