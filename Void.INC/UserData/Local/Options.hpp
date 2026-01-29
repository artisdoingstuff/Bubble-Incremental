#pragma once

#include "../../Misc/Globals/GIncludes.hpp"

inline void saveSettings(
	bool renderEffects,
	bool quickStart
) {
	std::ofstream file("options.txt");

	if (file.is_open()) {
		file << (renderEffects ? "1" : "0") << std::endl;
		file << (quickStart ? "1" : "0") << std::endl;
		file.close();
	}
	if (!file.is_open()) std::cerr << "Unable to open options.txt." << std::endl;
}

inline void loadSettings(
	bool& renderEffects,
	bool& quickStart
) {
	std::ifstream file("options.txt");
	if (!file.is_open()) {
		std::cerr << "No options file found." << std::endl;
		renderEffects = true;
		quickStart = false;
		return;
	}

	std::string line;
	std::getline(file, line);
	renderEffects = (line == "1");
	std::getline(file, line);
	quickStart = (line == "1");
	file.close();
}