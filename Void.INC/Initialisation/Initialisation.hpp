#pragma once

#include "../Misc/Globals/GIncludes.hpp"

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
    // 1.0.0 patches
    dirTree.emplace_back("Patch_0", "Increases bits-to-byte rate to 1e-6.", 0.05L);
    dirTree.emplace_back("Patch_1", "Variance every 30s between x1.5 to x1.7 BPS.", 10.0L);
    dirTree.emplace_back("Patch_2", "x3 BPS and x2 clicks.", 50.0L);
    dirTree.emplace_back("Patch_3_1", "x5.5 BPS but lowers bits-to-byte rate to 3e-7.", 500.0L);
    dirTree.emplace_back("Patch_3_2", "Byte-scaled loop (+0.02% per Byte @ 10,000% Cap).", 1000.0L);
    // 1.0.2 patches
    dirTree.emplace_back("Patch_4_1", "x12 BPS and x3 clicks, but deprecates not_gate and bit_flipper", 5000.0L);
    dirTree.emplace_back("Patch_4_2", "Legacy Hardware Refactor (+5% per Logic Gate @ 500% Cap).", 12500.0L);
    dirTree.emplace_back("Patch_5_1", "x50 BPS and x1.5 Bytes, but disables all of Patch_1", 75000.0L);
    dirTree.emplace_back("Patch_5_2", "x100 BPS, x5 clicks, and a 10% discount.", 200000.0L);
    // 1.0.7 patches
    dirTree.emplace_back("Patch_3", "x4 BPS.", 350.0L);
    dirTree.emplace_back("Patch_4", "x8 BPS and x2 clicks.", 3000.0L);
    dirTree.emplace_back("Patch_5", "x35 BPS and a 5% discount.", 50000.0L);
    dirTree.emplace_back("Patch_1_1", "x1.5 BPS.", 8.0L);
    dirTree.emplace_back("Patch_1_2", "x1.5 clicks.", 10.0L);
    dirTree.emplace_back("Patch_!", "x60 BPS and x1.75 Bytes, but x0.8 clicks.", 100000.0L);
    dirTree.emplace_back("Patch_@", "x999 BPS but x0.5 clicks, and deprecates shift_register and null_comparator.", 2500000.0L);
    dirTree.emplace_back("Patch_6", "x250 BPS and increases bits-to-bytes rate to 5e-7.", 450000.0L);
    dirTree.emplace_back("Patch_7", "x450 BPS and +9 clicks.", 1750000.0L);
    dirTree.emplace_back("Patch_7_1", "x600 BPS but disables Patch_3_2 and x0.4 clicks", 3500000.0L);
    dirTree.emplace_back("Patch_7_2", "Byte-scaled loop (+0.1% per Byte @ 350,000% Cap).", 10000000.0L);
    dirTree.emplace_back("Patch_2_1", "x3.5 BPS.", 100.0L);
    dirTree.emplace_back("Patch_!!", "x6,500 BPS and 5,000 starting BPS, but deprecates bus_sniffer, stack_siphon, and parity_checker.", 22500000.0L);
    dirTree.emplace_back("Patch_A", "x22,500 BPS.", 250000000.0L);
    dirTree.emplace_back("Patch_B", "15% discount.", 1500000000.0L);
    dirTree.emplace_back("Patch_C", "x85,000 BPS.", 17500000000.0L);
    dirTree.emplace_back("Patch_B_1", "x4 Bytes but x0.1 and -5 clicks.", 4500000000.0L);
    dirTree.emplace_back("Patch_6_2", "x400 BPS and x2 Bytes.", 1250000.0L);
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