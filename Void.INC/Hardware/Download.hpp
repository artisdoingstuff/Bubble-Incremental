#pragma once

#include "../Misc/GIncludes.hpp"
#include "LogicGate.hpp"

inline std::pair<long double, int> getDownload(LogicGate& lg, long double currentBits, Download mode) {
    int n = 0;
    if (mode == Download::X1) n = 1;
    else if (mode == Download::X5) n = 5;
    else if (mode == Download::X10) n = 10;
    else if (mode == Download::X50) n = 50;
    else if (mode == Download::X100) n = 100;
    else if (mode == Download::MAX) {
        float r = logicGateInflation;
        n = static_cast<int>(std::log(1.0L - (currentBits * (1.0L - r) / lg.currentBits)) / std::log(r));
        if (n < 0) n = 0;
    }

    long double r = logicGateInflation;
    long double totalCost = lg.currentBits * (std::pow(r, n) - 1.0L) / (r - 1.0L);

    if (n <= 1) return { lg.currentBits, 1 };
    return { totalCost, n };
}