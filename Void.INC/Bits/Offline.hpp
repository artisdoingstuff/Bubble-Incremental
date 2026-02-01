#pragma once

#include "../Misc/Globals/GIncludes.hpp"
#include "../Hardware/LogicGate.hpp"
#include "../Initialisation/Initialisation.hpp"

inline void offline(time_t timestamp, long double& bits, long double& allBits, long double bitsPerSecond, long double hotfixMult) {
	time_t elapsedTime = time(nullptr) - timestamp;
    long double patch3_2Mult = 1.0L + (bytes * 0.002L);
    long double patch7_2Mult = 1.0L + (bytes * 0.01L);
    long double patchC_4Mult = 1.0L + (bytes * 0.1L);

	if (elapsedTime > 0) {
        long double eBPS = 0.L;
        for (size_t i = 0; i < logicGateList.size(); ++i) {
            long double indivMult = 1.0L;
            if (i < 7) {
                if (dirTree[21].patched) { // !!
                    logicGateList[i].bps = 0.L;
                }

                else if (dirTree[6].patched) { // 4_2
                    indivMult = 1.0L + (logicGateList[i].ver * 0.05L);
                    if (indivMult > 50.0L) indivMult = 50.0L;
                }
            }

            else if (i >= 7 && i < 14) {
                if (dirTree[28].patched) { // 6_1
                    indivMult = 1.0L + (logicGateList[i].ver * 0.2L);
                    if (indivMult > 100.0L) indivMult = 100.0L;
                }
            }
            eBPS += (logicGateList[i].bps * logicGateList[i].ver) * indivMult;
        }

        long double dirMult = 1.0L;

        if (dirTree[2].patched) dirMult *= 3.0L; // 2
        if (dirTree[3].patched) dirMult *= 5.5L; // 3_1
        if (dirTree[5].patched) dirMult *= 12.0L; // 4_1
        if (dirTree[7].patched) { // 5_1
            dirMult *= 50.0L;
        }
		if (dirTree[8].patched) dirMult *= 100.0L; // 5_2
        if (dirTree[9].patched) dirMult *= 4.0L; // 3
        if (dirTree[10].patched) dirMult *= 8.0L; // 4
        if (dirTree[11].patched) dirMult *= 35.0L; // 5
        if (dirTree[14].patched) dirMult *= 60.0L; // !
        if (dirTree[15].patched) dirMult *= 999.0L; // @
        if (dirTree[16].patched) dirMult *= 250.0L; // 6
        if (dirTree[17].patched) dirMult *= 450.0L; // 7
        if (dirTree[18].patched) { // 7_1
            dirMult *= 600.0L;
        }
        if (dirTree[20].patched) dirMult *= 3.5L; // 2_1
        if (dirTree[21].patched) dirMult *= 6500.0L; // !!
        if (dirTree[22].patched) dirMult *= 22500.0L; // A
        if (dirTree[24].patched) dirMult *= 85000.0L; // C
        if (dirTree[26].patched) dirMult *= 400.0L; // 6_2
        if (dirTree[27].patched) dirMult *= 4.5L; // 2_2
        if (dirTree[29].patched) dirMult *= 175000.0L; // C_1
        if (dirTree[30].patched) dirMult *= 950000.0L; // C_2
        if (dirTree[31].patched) dirMult *= 5500000.0L; // C_3
        if (dirTree[32].patched) { // C_4
            dirMult *= std::min(patchC_4Mult, 10000000.0L); patch7_2Mult = 1.0L;
        }

        if (dirTree[1].patched && dirTree[1].disabled == 0) dirMult *= 1.6L; // 1
        if (dirTree[12].patched && dirTree[12].disabled == 0) dirMult *= 1.5L; // 1_1
        if (dirTree[19].patched && dirTree[19].disabled == 0) { // 7_2
            dirMult *= std::min(patch7_2Mult, 3500.0L); patch3_2Mult = 1.0L;
        }
        if (dirTree[4].patched && dirTree[4].disabled == 0) dirMult *= std::min(patch3_2Mult, 100.0L); // 3_2
		
		long double offlineBits = (elapsedTime * eBPS * hotfixMult * dirMult) * 1;
		
		bits += offlineBits; allBits += offlineBits; accOfflineBits += offlineBits;
	}
}