#pragma once

#include "../../Misc/Globals/GIncludes.hpp"

inline void versionSave(std::string version) {
    json saveData;

    saveData["V"] = version;

    std::ofstream file("V.json");

    if (file.is_open()) file << std::setw(4) << saveData << std::endl; file.close();
	if (!file.is_open()) std::cerr << "Unable to open V.json." << std::endl;
}