#pragma once

#include "../Misc/GIncludes.hpp"
#include "../LogicGate/LogicGate.hpp"
#include "../Initialisation/Initialisation.hpp"

inline void offline(time_t timestamp, long double& bits, long double& allBits, long double bitsPerSecond, long double hotfixMult) {
	time_t elapsedTime = time(nullptr) - timestamp;
    bool isOffline_1 = false;
    bool isOffline_3_2 = false;
	if (elapsedTime > 0) {
        long double eBPS = 0.L;
        for (size_t i = 0; i < logicGateList.size(); ++i) { // 4_2
            long double indivMult = 1.0L;
            if (dirTree[6].patched && i < 7) {
                indivMult = 1.0L + (logicGateList[i].ver * 0.05L);
                if (indivMult > 50.0L) indivMult = 50.0L;
            }

            eBPS += (logicGateList[i].bps * logicGateList[i].ver) * indivMult;
        }

        long double dirMult = 1.0L;

        if (dirTree[2].patched) dirMult *= 2.0L; // 2
        if (dirTree[3].patched) dirMult *= 5.5L; // 3_1
        if (dirTree[5].patched) dirMult *= 12.0L; // 4_1
        if (dirTree[7].patched) { // 5_1
            dirMult *= 50.0L; isOffline_1 = true;
        }
		if (dirTree[8].patched) dirMult *= 100.0L; // 5_2
        if (dirTree[9].patched) dirMult *= 4.0L; // 3
        if (dirTree[10].patched) dirMult *= 8.0L; // 4
        if (dirTree[11].patched) dirMult *= 35.0L; // 5
        if (dirTree[14].patched) dirMult *= 60.0L; // !
        if (dirTree[15].patched) dirMult *= 999.0L; // @
        if (dirTree[16].patched) dirMult *= 250.0L; // 6

        if (!isOffline_1) {
            if (dirTree[1].patched) dirMult *= 1.6L; // 1
            if (dirTree[12].patched) dirMult *= 1.5L; // 1_1
        }
        if (!isOffline_3_2) {
            if (dirTree[4].patched) { // 3_2
                long double patch3_2Mult = 1.0L + (bytes * 0.002L);
                dirMult *= std::min(patch3_2Mult, 100.0L);
            }
        }
		
		long double offlineBits = (elapsedTime * eBPS * hotfixMult * dirMult) * 1;
		
		bits += offlineBits; allBits += offlineBits;
	}
}