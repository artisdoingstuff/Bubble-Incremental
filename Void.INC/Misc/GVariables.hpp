#pragma once
#include "GIncludes.hpp"

// Global references
inline std::string voidVersion = "v1.0.2"; // Version control for the game (Updater purposes)

inline bool reinitialization = false;
inline bool initialization = false;
inline float loadingProgress = 0.0f;
float initTimer = 0.0f;

enum class ReinitState {
    IDLE,
    VORTEX_EXPANSION,
    VORTEX_SHRINK,
    LOADING_BAR,
    ROOTDIR
};

inline ReinitState currentReinitStep = ReinitState::IDLE;

enum class InitState {
    IDLE,
    LOADING_BAR,
	VORTEX_EXPANSION
};

inline InitState currentInitStep = InitState::IDLE;;

inline bool canClick = true; // Pre-initialization
inline bool canClickInit = false; // Post-reinitialization

enum class Directory { NONE, LOGIC_GATES, HOTFIXES, REINIT };
inline Directory currentDir = Directory::LOGIC_GATES;

inline sf::FloatRect logicFolderBounds;
inline sf::FloatRect hotfixFolderBounds;
inline sf::FloatRect reinitFolderBounds;

// Dynamic shenanigans
inline float costMult = 1.0f; // Change the cost of all logicGates
inline float byteMult = 1.0f; // Change the byte gain rate from REINIT(); specifically (does NOT affect conversion)

// Inflation multipliers
inline float logicGateInflation = 1.135f; // Inflates the cost of logicGate (base*(1.135^amt))

// Important data
inline long double bits = 0.0L; // Current bits
inline long double allBits = 0.0L; // Total accumulated bits
inline long double allClickedBits = 0.0L; // Total accumulated bits from clicks

inline long double hotfixMult = 1.0L; // Multiplier for hotfixes (upgrades)

inline long double bitsPerSecond = 0.0L; // Current bits per second
inline long double bitsPerClick = 1.0L; // Bits per click
inline long double clickMultiplier = 1.0L; // Click multiplier

inline long double bytes = 0.0L; // Current bytes (from Re-initializing)
inline long long timesInitialized = 0LL; // Re-initializing is "Ascending"
inline long double bitsToBytesRate = 1e-8; // Rate of bits to bytes conversion rate
inline long double bitMultiplier = 1.0L; // Multiplier for bits per second (from root patches)

inline float patch_1Mult = 1.0f; // Multiplier from Patch_1
inline float patch_3_2Mult = 1.0L; // Multiplier from Patch_3_2
inline bool showConfirmPopup = false;

inline long double malbytes = 0.0L; // Malicious bytes (for 1.1 use)

inline sf::Font jetBrainsMono("Assets/Font/JetBrainsMono.ttf");