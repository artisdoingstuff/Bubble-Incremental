#include "Misc/Globals/GAudio.hpp"
#include "Misc/Globals/GFunctions.hpp"
#include "Misc/Globals/GIncludes.hpp"
#include "Misc/Globals/GVariables.hpp"

#include "Bits/Format.hpp"
#include "Bits/Offline.hpp"

#include "Corruption/Corruption.hpp"

#include "Effects/Bits.hpp"

#include "Hardware/Hotfixes.hpp"
#include "Hardware/Logic.hpp"

#include "Initialisation/Initialisation.hpp"
#include "Initialisation/Re-initialisation.hpp"

#include "UI/Core/Directory.hpp"
#include "UI/Core/Download.hpp"
#include "UI/Core/Terminal.hpp"
#include "UI/Core/Kernel.hpp"
#include "UI/Extra/Loading.hpp"
#include "UI/Extra/Offline.hpp"
#include "UI/Extra/Options.hpp"
#include "UI/Extra/Star.hpp"
#include "UI/Extra/Start.hpp"

#include "UserData/Local/Loading.hpp"
#include "UserData/Local/Options.hpp"
#include "UserData/Local/Saving.hpp"
#include "UserData/Local/Version.hpp"

int main() {
	if (std::filesystem::exists("updater.exe")) std::system("updater.exe");
	if (!std::filesystem::exists("updater.exe")) std::cout << "updater.exe not detected, skipping..." << std::endl;

	sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "Void.INC | " + voidVersion, sf::State::Fullscreen);
	window.setFramerateLimit(60);
	window.setIcon(sf::Image("icon.png"));

	sf::Vector2f centre = { window.getSize().x / 2.f, window.getSize().y / 2.f };
	sf::Vector2f sPos = { window.getSize().x * 0.7f, window.getSize().y / 2.f };

	time_t timeEnd = 0;

	sf::RectangleShape clickRect;
	clickRect.setSize(sf::Vector2f(256, 256));
	clickRect.setOrigin(sf::Vector2f(clickRect.getSize().x / 2, clickRect.getSize().y / 2));
	clickRect.setPosition(centre);
	clickRect.setOutlineColor(sf::Color::Black);
	clickRect.setOutlineThickness(5);
	sf::Vector2f clickAreaSize = clickRect.getSize();

	Star star;
	star.star = sf::VertexArray(sf::PrimitiveType::LineStrip, 4000);
	float starScale = 1.f;

	Cog cog;

	sf::VertexArray lines(sf::PrimitiveType::Lines);
	for (int i = 0; i < window.getSize().y; i += 4) {
		lines.append(sf::Vertex{ sf::Vector2f(0.f, (float)i), sf::Color(255, 255, 255, 40) });
		lines.append(sf::Vertex{ sf::Vector2f((float)window.getSize().x, (float)i), sf::Color(255, 255, 255, 40)});
	}

	sf::Text bitsText(jetBrainsMono);
	bitsText.setCharacterSize(36);
	bitsText.setFillColor(sf::Color(243,238,225));

	sf::Text bitsPerSecondText(jetBrainsMono);
	bitsPerSecondText.setCharacterSize(20);
	bitsPerSecondText.setFillColor(sf::Color(243, 238, 225));

	sf::Text bytesText(jetBrainsMono);
	bytesText.setCharacterSize(36);
	bytesText.setFillColor(sf::Color(0, 255, 150));

	sf::Text malbitsText(jetBrainsMono);
	malbitsText.setCharacterSize(20);
	malbitsText.setFillColor(sf::Color(255, 50, 0));

	initLogicGates();
	initHotfixes();
	initDirTree();
	initAudio();
	positionTreeNodes(window.getSize());
	load(timeEnd, bits, bytes, allBits, allClickedBits, allBytes, bitsPerSecond, hotfixMult, timesInitialised, malbits, malbytes, allMalbits, allMalbytes, timesCorrupted, currentCorruption, logicGateList, hotfixList, dirTree);
	loadOptions(renderEffects, quickStart, muteAll, muteSFX, muteAmbience);
	offline(timeEnd, bits, allBits, bitsPerSecond, hotfixMult);
	
	while (window.isOpen()) {
		if (dirTree[1].patched && patch_1Clock.getElapsedTime().asSeconds() >= 30.f && dirTree[1].disabled == 0) { // 1
			patch_1Mult = 1.5f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (1.7f - 1.5f)));
			patch_1Clock.restart();
		}

		bitsToBytesRate = 1e-8L;
		if (dirTree[0].patched) bitsToBytesRate = 1e-6L; // 0

		bitMultiplier = 1.0L;
		patch_3_2Mult = 1.0L;
		patch_7_2Mult = 1.0L;
		clickMultiplier = 1.0L;
		bitsPerClick = 1.0L;
		costMult = 1.0f;
		byteMultiplier = 1.0f;

		if (dirTree[2].patched) { // 2
			bitMultiplier *= 3.0L; clickMultiplier *= 2.0L;
		}
		if (dirTree[3].patched) { // 3_1
			bitMultiplier *= 5.5L; bitsToBytesRate = 3e-7L;
		}
		if (dirTree[4].patched && dirTree[4].disabled == 0) { // 3_2
			long double patch3_2Mult = 1.0L + (bytes * 0.002L);
			patch_3_2Mult = std::min(patch3_2Mult, 100.0L);
		}
		if (dirTree[5].patched) { // 4_1
			bitMultiplier *= 12.0L; clickMultiplier *= 3.0L;
			for (int i = 0; i < 2; ++i) {
				logicGateList[i].bps = 0.L;
			}
		}
		if (dirTree[6].patched) { // 4_2
			bitsPerSecond = 0.L;
			for (size_t i = 0; i < logicGateList.size(); ++i) {
				long double indivMult = 1.0L;
				if (i < 7) {
					indivMult = 1.0L + (static_cast<long double>(logicGateList[i].ver) * 0.05L);
					if (indivMult > 50.0L) indivMult = 50.0L;
				}

				bitsPerSecond += (logicGateList[i].bps * logicGateList[i].ver) * indivMult;
			}
		}
		if (dirTree[7].patched) { // 5_1
			bitMultiplier *= 50.0L; byteMultiplier *= 1.5f;
			dirTree[1].disabled = 1; dirTree[12].disabled = 1; dirTree[13].disabled = 1;
		}
		if (dirTree[8].patched) { // 5_2
			bitMultiplier *= 100.0L; clickMultiplier *= 5.0L; costMult *= 0.9f;
		}
		if (dirTree[9].patched) bitMultiplier *= 4.0L; // 3
		if (dirTree[10].patched) { // 4
			bitMultiplier *= 8.0L; clickMultiplier *= 2.0L;
		}
		if (dirTree[11].patched) { // 5
			bitMultiplier *= 35.0L; costMult *= 0.95f;
		}
		if (dirTree[12].patched && dirTree[12].disabled == 0) bitMultiplier *= 1.5L; // 1_1
		if (dirTree[13].patched && dirTree[13].disabled == 0) clickMultiplier *= 1.5L; // 1_2
		if (dirTree[14].patched) bitMultiplier *= 60.0L; clickMultiplier *= 0.8L; byteMultiplier *= 1.75f; // !
		if (dirTree[15].patched) { // @
			bitMultiplier *= 999.0L; clickMultiplier *= 0.5L;
			for (int i = 0; i < 4; ++i) {
				logicGateList[i].bps = 0.L;
			}
		}
		if (dirTree[16].patched) { // 6
			bitMultiplier *= 250.0L; bitsToBytesRate = 5e-7L;
		}
		if (dirTree[17].patched) { // 7
			bitMultiplier *= 450.0L; bitsPerClick += 9.0L;
		}
		if (dirTree[18].patched) { // 7_1
			bitMultiplier *= 600.0L; clickMultiplier *= 0.4L;  dirTree[4].disabled = 1;
		}
		if (dirTree[19].patched && dirTree[19].disabled == 0) { // 7_2
			long double patch7_2Mult = 1.0L + (bytes * 0.01L);
			patch_7_2Mult = std::min(patch7_2Mult, 3500.0L);
			patch_3_2Mult = 1.0L;
		}
		if (dirTree[20].patched) { // 2_1
			bitMultiplier *= 3.5L;
		}
		if (dirTree[21].patched) { // !!
			bitMultiplier *= 6500.0L;  bitsFromPatch = 5000.0L;
			for (int i = 0; i < 7; ++i) {
				logicGateList[i].bps = 0.L;
			}
		}
		if (dirTree[22].patched) bitMultiplier *= 22500.0L; // A
		if (dirTree[23].patched) costMult *= 0.85f; // B
		if (dirTree[24].patched) bitMultiplier *= 85000.0L; // C
		if (dirTree[25].patched) { // B_1
			byteMultiplier *= 4.0L; clickMultiplier *= 0.1L; bitsPerClick -= 5.0L;
		}
		if (dirTree[26].patched) { // 6_2
			bitMultiplier *= 400.0L; byteMultiplier *= 2.0L;
		}
		if (dirTree[27].patched) { // 2_2
			bitMultiplier *= 4.5L;
		}
		if (dirTree[28].patched) { // 6_1
			bitsPerSecond = 0.L;
			for (size_t i = 6; i < logicGateList.size(); ++i) {
				long double indivMult = 1.0L;
				if (i < 14) {
					indivMult = 1.0L + (static_cast<long double>(logicGateList[i].ver) * 0.2L);
					if (indivMult > 100.0L) indivMult = 100.0L;
				}

				bitsPerSecond += (logicGateList[i].bps * logicGateList[i].ver) * indivMult;
			}
			byteMultiplier *= 0.8f;
		}
		if (dirTree[29].patched) { // C_1
			bitMultiplier *= 175000.0L; clickMultiplier *= 1.1L;
		}
		if (dirTree[30].patched) { // C_2
			bitMultiplier *= 950000.0L; byteMultiplier *= 2.0L;
		}
		if (dirTree[31].patched) { // C_3
			bitMultiplier *= 5500000.0L; costMult *= 0.85f;
		}
		if (dirTree[32].patched) { // C_4
			long double patchC_4Mult = 1.0L + (bytes * 0.1L);
			patch_C_4Mult = std::min(patchC_4Mult, 10000000.0L);
			patch_7_2Mult = 1.0L;
			dirTree[19].disabled = 1;
		}

		long double realBitsPerSecond = (bitsPerSecond * hotfixMult * bitMultiplier * patch_1Mult) * patch_3_2Mult * patch_7_2Mult * patch_C_4Mult + bitsFromPatch;
		float deltaTime = deltaClock.restart().asSeconds();
		float elapsedTime = elapsedClock.getElapsedTime().asSeconds();

		bits += realBitsPerSecond * deltaTime; allBits += realBitsPerSecond * deltaTime;

		starScale += (1.f - starScale) * 0.1f;
		updateStar(star, centre, elapsedTime, starScale, allBits);

		bitsText.setString("-" + format(bits) + " Bits");
		centreText(bitsText, { clickRect.getPosition().x, clickRect.getPosition().y + 400 });

		bitsPerSecondText.setString("-" + format(realBitsPerSecond, true) + " Bits per Second");
		centreText(bitsPerSecondText, { clickRect.getPosition().x, clickRect.getPosition().y + 440 });

		bytesText.setString("-" + format(bytes, true) + " Bytes");
		centreText(bytesText, { clickRect.getPosition().x, clickRect.getPosition().y + 400 });

		if (allBits >= 1e30) {
			malbitsText.setString("-" + format(malbits) + " Malbits");
			centreText(malbitsText, { clickRect.getPosition().x, clickRect.getPosition().y + 470 });
		}

		sf::RenderStates states;
		states.blendMode = sf::BlendAdd;

		updateVolume();

		window.clear(sf::Color::Black);

		if (showStart && !quickStart) {
			playMusic("menu");
			drawStartUI(window, states, star, elapsedTime, deltaTime, sPos, centre);
		}

		if (start && !quickStart) {
			if (currentStartStep == StartState::IDLE) currentStartStep = StartState::TRANSITION;
			timer += deltaTime;
			canClickStart = false;

			switch (currentStartStep) {
			case StartState::TRANSITION: {
				float t = std::min(1.f, timer / 1.0f);
				float ease = t * t * (0.5f * t);
				sPos = sPos + (centre - sPos) * ease;

				if (timer >= 1.0f) {
					sPos = centre;
					showStart = false;
					showOffline = true;
					start = false;
					canClick = true;
					timer = 0.0f;
					currentStartStep = StartState::IDLE;
				}
				break;
			}
			}
		}

		if (!showStart || quickStart) {
			ambienceRandom();
			if (quickStart) {
				showStart = false;
				canClick = true;
			}
			if (!reinitialisation && !initialisation) {
				playMusic("main");
				window.draw(bitsText);
				window.draw(bitsPerSecondText);
				window.draw(malbitsText);

				if (renderEffects) {
					updateStream(window, centre, deltaTime, 2);
					for (auto& d : dataStream) {
						window.draw(d.bit);
					}
				}

				updateLogicGateUI(window, allBits);

				window.draw(star.star, states);

				drawTerminalUI(window, bits, allBits, deltaTime);

				if (showOffline) drawOfflineUI(window, centre);
				if (showConfirmPopup) drawConfirmPopup(window, reinitialisation, centre);
			}
		}

		if (reinitialisation) {
			if (currentReinitStep == ReinitState::IDLE) {
				currentReinitStep = ReinitState::VORTEX_EXPANSION;
				playMusic("loading");
				canClick = false;
			}
			timer += deltaTime;
			sf::View shakeView = window.getDefaultView();

			switch (currentReinitStep) {
			case ReinitState::VORTEX_EXPANSION: {
				starScale += (20.f - starScale) * 0.05f;

				float intensity = (starScale / 20.f) * 15.f;
				float offsetX = (std::rand() % 100 - 50) / 50.f * intensity;
				float offsetY = (std::rand() % 100 - 50) / 50.f * intensity;
				shakeView.move({ offsetX, offsetY });

				window.setView(shakeView);
				window.draw(star.star, states);

				if (timer >= 0.5f) currentReinitStep = ReinitState::VORTEX_SHRINK;
				break;
			}

			case ReinitState::VORTEX_SHRINK: {
				starScale += (0.f - starScale) * 0.15f;
				shakeView.move({ (std::rand() % 10 - 5) / 2.f, (std::rand() % 10 - 5) / 2.f });

				window.setView(shakeView);
				window.draw(star.star, states);

				if (timer >= 0.7f) {
					starScale = -10.f;
					currentReinitStep = ReinitState::LOADING_BAR;
				}
				break;
			}

			case ReinitState::LOADING_BAR:
				loadingProgress = (timer - 0.7f) / 5.0f;

				if (loadingProgress >= 1.0f) {
					loadingProgress = 1.0f;
					currentReinitStep = ReinitState::DIR;
					playMusic("directory");
				}

				drawLoadingUI(window, loadingProgress);
				break;

			case ReinitState::DIR:
				window.setView(window.getDefaultView());
				loadingProgress = 0.0f;
				timer = 0.0f;
				canClickInit = true;

				ambienceRandom();

				if (renderEffects) {
					updateStream(window, centre, deltaTime, 1);
					for (auto& d : dataStream) {
						window.draw(d.bit);
					}
				}

				drawTreeLines(window);
				drawDirTreeUI(window);
				
				drawTerminalUI(window, bits, allBits, deltaTime);

				window.draw(bytesText);
				break;
			}
		}

		if (initialisation) {
			if (currentInitStep == InitState::IDLE) {
				currentInitStep = InitState::LOADING_BAR;
				playMusic("loading");
			}

			timer += deltaTime;
			sf::View shakeView = window.getDefaultView();

			switch (currentInitStep) {
			case InitState::LOADING_BAR: {
				loadingProgress = timer / 3.0f;
				drawLoadingUI(window, loadingProgress);

				if (loadingProgress >= 1.0f) {
					loadingProgress = 1.0f;
					currentInitStep = InitState::VORTEX_EXPANSION;
					starScale = 0.1f;
				}
				break;
			}

			case InitState::VORTEX_EXPANSION: {
				starScale += (1.f - starScale) * 0.1f;

				float intensity = (1.f - starScale) * 20.f;
				float offsetX = (std::rand() % 100 - 50) / 50.f * intensity;
				float offsetY = (std::rand() % 100 - 50) / 50.f * intensity;
				shakeView.move({ offsetX, offsetY });
				window.setView(shakeView);

				window.draw(star.star, states);

				if (timer >= 3.5f) {
					window.setView(window.getDefaultView());
					loadingProgress = 0.0f;
					timer = 0.0f;
					canClick = true;

					starScale = 1.f;

					currentInitStep = InitState::IDLE;
					initialisation = false;
				}
				break;
			}
			}
		}
		
		if (!showStart) {
			updateCog(cog, window, deltaTime, showOptions);
			window.draw(cog.cog);
		}
		if (showOptions) drawOptionsUI(window, showOptions, centre);

		window.draw(lines);

		window.display();

		while (const std::optional gameEvent = window.pollEvent()) {
			sf::Vector2f winSize = (sf::Vector2f)window.getSize();
			if (gameEvent->is<sf::Event::Closed>()) {
				save(time(nullptr), bits, bytes, allBits, allClickedBits, allBytes, bitsPerSecond, hotfixMult, timesInitialised, malbits, malbytes, allMalbits, allMalbytes, timesCorrupted, currentCorruption, logicGateList, hotfixList, dirTree);
				saveOptions(renderEffects, quickStart, muteAll, muteSFX, muteAmbience);
				versionSave(voidVersion);
				window.close();
			}

			if (gameEvent->is<sf::Event::TextEntered>()) {
				const auto& textEvent = gameEvent->getIf<sf::Event::TextEntered>();

				if (activeTab == Tab::LOGS && !logsUnlocked) {
					if (textEvent->unicode == '\b') {
						if (!logInput.empty()) logInput.pop_back();
						playSFX("button");
					}
					else if (textEvent->unicode < 128 && textEvent->unicode != '\r' && textEvent->unicode != '\n') {
						logInput += static_cast<char>(textEvent->unicode);
						playSFX("button");
					}
				}
			}

			if (gameEvent->is<sf::Event::MouseButtonPressed>()) {
				const auto& mouseEvent = gameEvent->getIf<sf::Event::MouseButtonPressed>();
				sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

				if (mouseEvent->button == sf::Mouse::Button::Left && canClick) {
					if (clickRect.getGlobalBounds().contains(mousePos)) {
						long double bitsClicked = bitsPerClick * clickMultiplier * (1 + realBitsPerSecond * 0.05f);
						starScale = 1.1f; bits += bitsClicked; allBits += bitsClicked; allClickedBits += bitsClicked;
						playSFX("heartbeat");
					}

					if (mousePos.y > winSize.y - 60.f) {
						if (sf::FloatRect({ 10, winSize.y - 50.f }, { 180, 40 }).contains(mousePos)) {
							activeTab = (activeTab == Tab::LOGIC) ? Tab::NONE : Tab::LOGIC; playSFX("button");
						}
						else if (sf::FloatRect({ 200, winSize.y - 50.f }, { 180, 40 }).contains(mousePos)) {
							activeTab = (activeTab == Tab::HOTFIX) ? Tab::NONE : Tab::HOTFIX; playSFX("button");
						}
						else if (sf::FloatRect({ 390, winSize.y - 50.f }, { 180, 40 }).contains(mousePos)) {
							activeTab = (activeTab == Tab::STATS) ? Tab::NONE : Tab::STATS; playSFX("button");
						}
						else if (sf::FloatRect({ 580, winSize.y - 50.f }, { 180, 40 }).contains(mousePos)) {
							activeTab = (activeTab == Tab::REINIT) ? Tab::NONE : Tab::REINIT; playSFX("button");
						}
						else if (sf::FloatRect({ 770, winSize.y - 50.f }, { 180, 40 }).contains(mousePos)) {
							activeTab = (activeTab == Tab::LOGS) ? Tab::NONE : Tab::LOGS; playSFX("button");
						}
					}

					if (activeTab == Tab::LOGIC) {
						for (auto& lg : logicGateList) {
							if (lg.costType == currentLogicMode && lg.rect.getGlobalBounds().contains(mousePos)) {
								long double& activeCurrency = (lg.costType == Currency::BIT) ? bits : malbits;
								auto [totalCost, amount] = getDownload(lg, activeCurrency, currentBuy);
								if (activeCurrency >= totalCost && amount > 0) {
									activeCurrency -= totalCost;
									lg.ver += amount;
									bitsPerSecond += (lg.bps * amount);
									lg.currentCost = lg.baseCost * std::pow(logicGateInflation, lg.ver) * costMult;
									playSFX("installed");
								}
							}
						}
						showConfirmPopup = false;
					}
					else if (activeTab == Tab::HOTFIX) {
						size_t startIdx = hotfixPage * HF_PER_PAGE;
						size_t endIdx = std::min(startIdx + HF_PER_PAGE, hotfixList.size());
						for (size_t i = startIdx; i < endIdx; ++i) {
							auto& hf = hotfixList[i];
							if (hf.written == 0 && hf.rect.getGlobalBounds().contains(mousePos)) {
								if (bits >= hf.bits) {
									bits -= hf.bits;
									hf.written = 1;
									hotfixMult += hf.bitMult;
									playSFX("installed");
								}
							}
						}
						showConfirmPopup = false;
					}
					else if (activeTab == Tab::STATS) showConfirmPopup = false;
					else if (activeTab == Tab::REINIT) showConfirmPopup = true;
					else if (activeTab == Tab::LOGS) showConfirmPopup = false;
					else if (activeTab == Tab::NONE) showConfirmPopup = false;
				}

				if (mouseEvent->button == sf::Mouse::Button::Left && canClickInit) {
					if (mousePos.y > winSize.y - 60.f) {
						if (sf::FloatRect({ 10, winSize.y - 50.f }, { 180, 40 }).contains(mousePos)) {
							activeTab = (activeTab == Tab::INIT) ? Tab::NONE : Tab::INIT; playSFX("button");
						}
					}
					
					if (activeTab == Tab::INIT) {
						currentReinitStep = ReinitState::IDLE;
						reinitialisation = false;
						initialisation = true;
						canClickInit = false;
						playSFX("button");
					}

					for (size_t i = 0; i < dirTree.size(); ++i) {
						auto& patch = dirTree[i];

						sf::Vector2f diff = mousePos - patch.pos;
						float distanceSquared = (diff.x * diff.x) + (diff.y * diff.y);

						if (patch.patched == 0 && !patch.disabled && distanceSquared <= (30.f * 30.f)) {
							if (bytes >= patch.bytes) {
								bytes -= patch.bytes;
								patch.patched = 1;
								playSFX("patched");

								if (patch.name == "Patch_0") bitsToBytesRate = 1e-6L;
								else if (patch.name == "Patch_3_1") bitsToBytesRate = 3e-7L;
								else if (patch.name == "Patch_6") bitsToBytesRate = 5e-7L;
							}
						}
					}
				}
			}
		}
	}
}