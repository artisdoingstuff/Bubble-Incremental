#pragma once

#include "../../Misc/Globals/GIncludes.hpp"

inline void save(
	time_t timestamp,
	long double bits,
	long double bytes,
	long double allBits,
	long double allClickedBits,
	long double bitsPerSecond,
	long double hotfixMult,
	long long timesInitialised,
	std::vector<LogicGate>& logicGate,
	std::vector<Hotfix>& hotfix,
	std::vector<DirNodes>& root
) {
	json saveData;

	auto round = [](long double v) -> long double {return std::round(v * 100.0) / 100.0; };

	saveData["T"] = timestamp;
	saveData["B"] = round(bits);
	saveData["BY"] = round(bytes);
	saveData["AB"] = round(allBits);
	saveData["ACB"] = round(allClickedBits);
	saveData["BPS"] = round(bitsPerSecond);
	saveData["HFM"] = round(hotfixMult);
	saveData["TINIT"] = timesInitialised;
	saveData["LOGIC"] = logicGate;
	saveData["HOTFIX"] = hotfix;
	saveData["ROOT"] = root;

	std::ofstream file("save.json");

	if (file.is_open()) file << std::setw(4) << saveData << std::endl; file.close();
	if (!file.is_open()) std::cerr << "Unable to open save.json." << std::endl;
}