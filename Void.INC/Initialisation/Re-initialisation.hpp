#pragma once

#include "../Misc/GIncludes.hpp"
#include "../LogicGate/LogicGate.hpp"
#include "../LogicGate/Hotfixes.hpp"

inline void resetProgress() {
    for (auto& lg : logicGateList) {
        lg.ver = 0;
        lg.currentBits = lg.baseBits;
    }

    for (auto& hf : hotfixList) {
        hf.written = 0;
    }

    bits = 0.0L;
    bitsPerSecond = 0.0L;
    hotfixMult = 1.0L;
    scrollOffset = 0.0f;
}

inline long double getPendingBytes(long double bits) {
    auto round = [](long double v) -> long double {return std::round(v * 100.0) / 100.0; };
    if (bits < 5000000.0L) return 0;
    return round(bits * bitsToBytesRate);
}

inline void drawConfirmPopup(sf::RenderWindow& window, bool& startInit) {
    sf::RectangleShape overlay({ (float)window.getSize().x, (float)window.getSize().y });
    overlay.setFillColor(sf::Color(20, 20, 20, 200));

    sf::RectangleShape box({ 400.f, 200.f });
    box.setOrigin({ 200.f, 100.f });
    box.setPosition({ window.getSize().x / 2.f, window.getSize().y / 2.f });
    box.setFillColor(sf::Color(20, 20, 20));
    box.setOutlineColor(sf::Color::Red);
    box.setOutlineThickness(2);

    sf::Text warnText(jetBrainsMono, "REINITIALISING WILL WIPE\nALL CURRENT DATA.\n\nPROCEED?", 18);
    warnText.setOrigin({ warnText.getGlobalBounds().size.x / 2.f, 0.f });
    warnText.setPosition(sf::Vector2f(box.getPosition().x, box.getPosition().y - 70.f));

    sf::Text yesText(jetBrainsMono, "[ YES ]", 18);
    yesText.setPosition(sf::Vector2f(box.getPosition().x - 100.f, box.getPosition().y + 40.f));
    yesText.setFillColor(sf::Color::Green);

    sf::Text noText(jetBrainsMono, "[ NO ]", 18);
    noText.setPosition(sf::Vector2f(box.getPosition().x + 20.f, box.getPosition().y + 40.f));
    noText.setFillColor(sf::Color::Red);

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    window.draw(overlay);
    window.draw(box);
    window.draw(warnText);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (yesText.getGlobalBounds().contains(mousePos)) {
            startInit = true;
            bytes += getPendingBytes(bits);
            resetProgress();
            timesInitialised++;
            showConfirmPopup = false;
        }
        if (noText.getGlobalBounds().contains(mousePos)) {
            showConfirmPopup = false;
        }
    }

    window.draw(yesText);
    window.draw(noText);
}