#pragma once

#include "../Misc/GIncludes.hpp"

struct LogicGate {
    std::string name;
    std::string desc;
    long double baseBits;
    long double currentBits;
    long double bps;
    int ver = 0;
    sf::RectangleShape rect;

    LogicGate() : baseBits(0), currentBits(0), bps(0) {}

    LogicGate(std::string n, std::string d, long double bc, long double cc, long double bps)
        : name(n), desc(d), baseBits(bc), currentBits(cc), bps(bps) {
        rect.setSize(sf::Vector2f(350.f, 70.f));
        rect.setOutlineColor(sf::Color(243, 238, 225));
        rect.setOutlineThickness(2.f);
    }
};

inline std::vector<LogicGate> logicGateList;
inline float scrollOffset = 0.f;
inline float logicGateHeight = 70.f;
inline float logicGateSpacing = 10.f;
inline float uiWidth = 350.f;

inline void initLogicGates() {
    // EARLY-GAME
    logicGateList.emplace_back("not_gate", "Inverts existence into non-existence.", 10.L, 10.L, 1.L);
    logicGateList.emplace_back("bit_flipper", "Forces ones into zeros.", 60.L, 60.L, 4.L);
    logicGateList.emplace_back("shift_register", "Pushes data into the abyss.", 250.L, 250.L, 15.L);
    logicGateList.emplace_back("null_comparator", "Validates the lack of data.", 1200.L, 1200.L, 65.L);
    logicGateList.emplace_back("bus_sniffer", "Captures and deletes stray signals.", 5000.L, 5000.L, 240.L);
    logicGateList.emplace_back("stack_siphon", "Drains the system's memory reserves.", 20000.L, 20000.L, 900.L);
    logicGateList.emplace_back("parity_checker", "Corrects reality until it's empty.", 85000.L, 85000.L, 3500.L);
    logicGateList.emplace_back("instruction_bleeder", "Slowly leaks the CPU's logic.", 350000.L, 350000.L, 13000.L);
    logicGateList.emplace_back("heap_corruptor", "Destroys the structure of bits.", 1200000.L, 1200000.L, 42000.L);

    // ASCENSION ENTRY (5M)
    logicGateList.emplace_back("packet_dropper", "Ensures nothing ever arrives.", 5000000.L, 5000000.L, 160000.L);

    // MID-GAME
    logicGateList.emplace_back("buffer_overflow", "Spills data into unallocated voids.", 15000000.L, 15000000.L, 450000.L);
    logicGateList.emplace_back("logic_bomb", "A timed explosion of nothingness.", 40000000.L, 40000000.L, 1100000.L);
    logicGateList.emplace_back("hardware_interrupt", "Halts the flow of reality.", 100000000.L, 100000000.L, 2600000.L);
    logicGateList.emplace_back("deadlock_thread", "Infinite waiting for zero output.", 220000000.L, 220000000.L, 5500000.L);
    logicGateList.emplace_back("kernel_overload", "Total system failure for profit.", 450000000.L, 450000000.L, 10500000.L);

    // END-GAME
    logicGateList.emplace_back("write_blocker", "Prevents any new data creation.", 1000000000.L, 1000000000.L, 22000000.L);
    logicGateList.emplace_back("entropy_driver", "Accelerates the decay of code.", 2500000000.L, 2500000000.L, 50000000.L);
    logicGateList.emplace_back("negative_bus", "Transports bits that shouldn't exist.", 6000000000.L, 6000000000.L, 115000000.L);
    logicGateList.emplace_back("ghost_process", "Executes without consuming resources.", 15000000000.L, 15000000000.L, 275000000.L);
    logicGateList.emplace_back("root_nullifier", "The final line of code.", 50000000000.L, 50000000000.L, 850000000.L);
}

inline void updateLogicGateUI(sf::RenderWindow& window, long double allBits) {
    const float margin = 40.f;
    float startX = window.getSize().x - uiWidth - margin;
    float startY = 105.f;

    int visibleCount = 0;
    for (size_t i = 0; i < logicGateList.size(); ++i) {
        if (logicGateList[i].ver > 0 || allBits >= logicGateList[i].baseBits) {
            float yPos = startY + (static_cast<float>(visibleCount) * (logicGateHeight + logicGateSpacing)) - scrollOffset;
            logicGateList[i].rect.setPosition({ startX, yPos });
            visibleCount++;
        }
    }
}

inline void to_json(json& j, const LogicGate& lg) {
    j = json{ {"n", lg.name}, {"v", lg.ver} };
}

inline void from_json(const json& j, LogicGate& lg) {
    j.at("n").get_to(lg.name);
    j.at("v").get_to(lg.ver);
}