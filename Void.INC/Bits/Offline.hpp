#pragma once

#include "../Misc/GIncludes.hpp"
#include "../LogicGate/LogicGate.hpp"
#include "../Re-initialization/Initialization.hpp"

inline void offline(time_t timestamp, long double& bits, long double& allBits, long double bitsPerSecond, long double hotfixMult) {
	time_t elapsedTime = time(nullptr) - timestamp;
	if (elapsedTime > 0) {
        long double eBPS = 0.L;
        for (size_t i = 0; i < logicGateList.size(); ++i) {
            long double indivMult = 1.0L;

            if (rootTree[6].patched && i < 7) {
                indivMult = 1.0L + (logicGateList[i].ver * 0.05L);
                if (indivMult > 50.0L) indivMult = 50.0L;
            }

            eBPS += (logicGateList[i].bps * logicGateList[i].ver) * indivMult;
        }

        long double rootMult = 1.0L;

        if (rootTree[7].patched) rootMult *= 50.0L;
        else {
            if (rootTree[1].patched) rootMult *= 1.6L;
            if (rootTree[2].patched) rootMult *= 2.0L;
        }

        if (rootTree[3].patched) rootMult *= 5.5L;

        if (rootTree[4].patched) {
            long double patch3_2Mult = 1.0L + (bytes * 0.002L);
            rootMult *= std::min(patch3_2Mult, 100.0L);
        }

        if (rootTree[5].patched) rootMult *= 12.0L;

		if (rootTree[8].patched) rootMult *= 75.0L;
		
		long double offlineBits = (elapsedTime * eBPS * hotfixMult * rootMult) * 1;
		
		bits += offlineBits; allBits += offlineBits;
	}
}