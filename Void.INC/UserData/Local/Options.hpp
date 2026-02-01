#pragma once

#include "../../Misc/Globals/GIncludes.hpp"

inline void saveOptions(
	bool renderEffects,
	bool quickStart,
	bool muteAll,
	bool muteSFX,
	bool muteAmbience
) {
	std::ofstream file("options.txt");

	if (file.is_open()) {
		file << (renderEffects ? "1" : "0") << std::endl;
		file << (quickStart ? "1" : "0") << std::endl;
		file << (muteAll ? "1" : "0") << std::endl;
		file << (muteSFX ? "1" : "0") << std::endl;
		file << (muteAmbience ? "1" : "0") << std::endl;
		file.close();
	}
	if (!file.is_open()) std::cerr << "Unable to open options.txt." << std::endl;
}

inline void loadOptions(
	bool& renderEffects,
	bool& quickStart,
	bool& muteAll,
	bool& muteSFX,
	bool& muteAmbience
) {
	std::ifstream file("options.txt");
	if (!file.is_open()) {
		std::cerr << "No options file found." << std::endl;
		renderEffects = true;
		quickStart = false;
		muteAll = false;
		muteSFX = false;
		muteAmbience = false;
		return;
	}

	std::string line;
	std::getline(file, line);
	renderEffects = (line == "1");
	std::getline(file, line);
	quickStart = (line == "1");
	std::getline(file, line);
	muteAll = (line == "1");
	std::getline(file, line);
	muteSFX = (line == "1");
	std::getline(file, line);
	muteAmbience = (line == "1");
	file.close();
}