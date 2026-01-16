#pragma once

#include "../Misc/GIncludes.hpp"

struct DirNodes {
    std::string name;
    std::string desc;
    long double bytes;
    int patched = 0;
    int disabled = 0;
    sf::CircleShape nodeCircle;
    sf::Vector2f pos;

    DirNodes(std::string n, std::string t, long double c) : name(n), desc(t), bytes(c) {
        nodeCircle.setRadius(30.f);
        nodeCircle.setOrigin({ 30.f, 30.f });
        nodeCircle.setFillColor(sf::Color(10, 20, 10));
        nodeCircle.setOutlineColor(sf::Color(0, 255, 150));
        nodeCircle.setOutlineThickness(2.f);
    }
};

inline std::vector<DirNodes> dirTree;
inline sf::Clock patch_1Clock;

inline void initDirTree() {
    if (!dirTree.empty()) return;
    dirTree.emplace_back("Patch_0", "Upgrade bit-to-byte protocols (1e-6).", 0.05L);
    dirTree.emplace_back("Patch_1", "Inject minute-based variance (x1.5 - x1.7).", 10.0L);
    dirTree.emplace_back("Patch_2", "Triples bits and doubles clicks.", 50.0L);
    dirTree.emplace_back("Patch_3_1", "x5.5 BPS but lowers byte rate to 3e-7.", 500.0L);
    dirTree.emplace_back("Patch_3_2", "Byte-scaled feedback loop (0.2% per Byte).", 1000.0L);
    dirTree.emplace_back("Patch_4_1", "x12 BPS, x3 clicks, deprecates Logic Gates 1 & 2.", 5000.0L);
    dirTree.emplace_back("Patch_4_2", "Legacy hardware performance refactor (+5% per Logic Gate).", 12500.0L);
    dirTree.emplace_back("Patch_5_1", "x50 BPS and x1.5 Bytes, but disables Patch_1 & Patch_2", 75000.0L);
    dirTree.emplace_back("Patch_5_2", "x100 BPS, x5 clicks... and 10% off on gates?", 200000.0L);
}

inline void drawInitButton(sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    sf::Vector2f btnPos(30.f, window.getSize().y - 60.f);

    sf::FloatRect hitbox({ btnPos.x, btnPos.y }, { 150.f, 40.f });
    bool isHovered = hitbox.contains(mousePos);

    std::string prompt = isHovered ? "> init.bat" : "  init.bat";
    sf::Text initTxt(jetBrainsMono, prompt, 22);
    initTxt.setPosition(btnPos);

    if (isHovered) {
        initTxt.setFillColor(sf::Color(243, 238, 225));
        sf::RectangleShape underline({ 100.f, 2.f });
        underline.setPosition({ btnPos.x + 28.f, btnPos.y + 28.f });
        underline.setFillColor(sf::Color(243, 238, 225));
        window.draw(underline);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && canClickInit) {
            currentReinitStep = ReinitState::IDLE;
            reinitialisation = false;
            initialisation = true;
            canClickInit = false;
        }
    }
    else {
        initTxt.setFillColor(sf::Color(180, 180, 180));
    }

    window.draw(initTxt);
}

inline void to_json(json& j, const DirNodes& p) {
    j = json{ {"n", p.name}, {"p", p.patched} };
}

inline void from_json(const json& j, DirNodes& p) {
    j.at("n").get_to(p.name);
    j.at("p").get_to(p.patched);
}