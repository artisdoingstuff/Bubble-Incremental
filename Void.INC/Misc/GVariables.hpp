#pragma once
#include "GIncludes.hpp"

// Global references
inline std::string voidVersion = "v1.0.8-release"; // Version control for the game (Updater purposes)

inline bool reinitialisation = false;
inline bool initialisation = false;
inline bool showStart = true;
inline bool start = false;

inline float loadingProgress = 0.0f;
inline float timer = 0.0f;

inline sf::Clock deltaClock;
inline sf::Clock elapsedClock;
inline sf::Clock cooldown;

enum class ReinitState {
    IDLE,
    VORTEX_EXPANSION,
    VORTEX_SHRINK,
    LOADING_BAR,
    DIR
};
inline ReinitState currentReinitStep = ReinitState::IDLE;

enum class InitState {
    IDLE,
    LOADING_BAR,
	VORTEX_EXPANSION
};
inline InitState currentInitStep = InitState::IDLE;;

enum class StartState {
    IDLE,
    TRANSITION
};
inline StartState currentStartStep = StartState::IDLE;

inline bool canClickStart = true; // Start menu only
inline bool canClick = false; // Pre-reinitialisation
inline bool canClickInit = false; // Post-initialisation

enum class Tab { NONE, LOGIC, HOTFIX, REINIT };
inline Tab activeTab = Tab::NONE;
inline float tabProgress = 0.f;

enum class Download { X1, X5, X10, X50, X100, MAX };
inline Download currentBuy = Download::X1;

inline float pi = 3.14159f;

// Dynamic shenanigans
inline float costMult = 1.0f; // Change the cost of all logicGates
inline float byteMultiplier = 1.0f; // Change the byte gain rate from reinit.bat specifically (does NOT affect conversion)

// Inflation multipliers
inline float logicGateInflation = 1.135f; // Inflates the cost of logicGate (base*(1.135^amt))
inline float hotfixGrowth = 1.1875f; // Recursive growth mult for hotfixes

// Important (and needed) data
inline long double bits = 0.0L; // Current bits
inline long double allBits = 0.0L; // Total accumulated bits
inline long double allClickedBits = 0.0L; // Total accumulated bits from clicks

inline long double hotfixMult = 1.0L; // Multiplier for hotfixes (upgrades)

inline long double bitsPerSecond = 0.0L; // Current bits per second
inline long double bitsPerClick = 1.0L; // Bits per click
inline long double clickMultiplier = 1.0L; // Click multiplier

inline long double bytes = 0.0L; // Current bytes (from Re-initializing)
inline long long timesInitialised = 0LL; // Re-initializing is "Ascending"
inline long double bitsToBytesRate = 1e-8L; // Rate of bits to bytes conversion rate
inline long double bitMultiplier = 1.0L; // Multiplier for bits per second (from root patches)

inline long double bitsFromPatch = 0.0L; // Applies to Patch_!! (currently)

inline float patch_1Mult = 1.0f; // Multiplier from Patch_1
inline float patch_3_2Mult = 1.0L; // Multiplier from Patch_3_2
inline float patch_7_2Mult = 1.0L; // Multiplier from Patch_7_2
inline bool showConfirmPopup = false;

inline long double malbytes = 0.0L; // Malicious bytes (for 1.1 use)

inline sf::Font jetBrainsMono("Assets/Font/JetBrainsMono.ttf");