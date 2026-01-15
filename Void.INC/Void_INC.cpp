#include "Misc/GFunctions.hpp"
#include "Misc/GIncludes.hpp"
#include "Misc/GVariables.hpp"

#include "Bits/Format.hpp"
#include "Bits/Offline.hpp"

#include "Effects/Bits.hpp"

#include "Initialisation/Initialisation.hpp"
#include "Initialisation/Re-initialisation.hpp"

#include "LogicGate/Hotfixes.hpp"
#include "LogicGate/LogicGate.hpp"

#include "UI/Directory.hpp"
#include "UI/Loading.hpp"
#include "UI/Terminal.hpp"
#include "UI/Star.hpp"

#include "UserData/Local/Loading.hpp"
#include "UserData/Local/Saving.hpp"
#include "UserData/Local/Version.hpp"

int main() {
	hideConsole();
	
	if (std::filesystem::exists("updater.exe")) std::system("updater.exe");
	if (!std::filesystem::exists("updater.exe")) std::cout << "updater.exe not detected, skipping..." << std::endl;

	sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "Void.INC | " + voidVersion, sf::State::Fullscreen);
	window.setFramerateLimit(60);
	window.setIcon(sf::Image("icon.png"));

	sf::Vector2f centre = { window.getSize().x / 2.f, window.getSize().y / 2.f };

	time_t timeEnd = 0;

	sf::Clock deltaClock;
	sf::Clock elapsedClock;

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

	initLogicGates();
	initHotfixes();
	initDirTree();
	positionTreeNodes(window.getSize());
	load(timeEnd, bits, bytes, allBits, allClickedBits, bitsPerSecond, hotfixMult, timesInitialised, logicGateList, hotfixList, dirTree);
	offline(timeEnd, bits, allBits, bitsPerSecond, hotfixMult);
	
	while (window.isOpen()) {
		if (dirTree[1].patched && patch_1Clock.getElapsedTime().asSeconds() >= 30.f && dirTree[1].disabled == 0) {
			patch_1Mult = 1.5f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (1.7f - 1.5f)));
			patch_1Clock.restart();
		}

		bitsToBytesRate = 1e-8L;
		if (dirTree[0].patched) bitsToBytesRate = 1e-6L;
		if (dirTree[3].patched) bitsToBytesRate = 3e-7L;

		bitMultiplier = 1.0L;
		patch_3_2Mult = 1.0L;
		clickMultiplier = 1.0L;
		costMult = 1.0f;
		if (dirTree[2].patched && dirTree[2].disabled == 0) {
			bitMultiplier *= 3.0L; clickMultiplier *= 2.0L;
		}
		if (dirTree[3].patched) bitMultiplier *= 5.5L;
		if (dirTree[4].patched) {
			long double patch3_2Mult = 1.0L + (bytes * 0.002L);
			patch_3_2Mult = std::min(patch3_2Mult, 100.0L);
		}
		if (dirTree[5].patched) {
			bitMultiplier *= 12.0L; clickMultiplier *= 3.0L;
			for (int i = 0; i < 2; ++i) {
				logicGateList[i].bps = 0.L;
			}
		}
		if (dirTree[6].patched) {
			bitsPerSecond = 0.L;
			for (size_t i = 0; i < logicGateList.size(); ++i) {
				long double indivMult = 1.0L;
				if (i < 7) {
					indivMult = 1.0L + (static_cast<long double>(logicGateList[i].ver) * 0.05L);

					if (indivMult > 50.0L) {
						indivMult = 50.0L;
					}
				}

				bitsPerSecond += (logicGateList[i].bps * logicGateList[i].ver) * indivMult;
			}
		}
		if (dirTree[7].patched) {
			bitMultiplier *= 50.0L; byteMult *= 1.5f;
			dirTree[1].disabled = 1; dirTree[2].disabled = 1;
		}
		if (dirTree[8].patched) {
			bitMultiplier *= 75.0L; clickMultiplier *= 10.0L; costMult *= 0.9f;
		}

		long double realBitsPerSecond = (bitsPerSecond * hotfixMult * bitMultiplier * patch_1Mult) * patch_3_2Mult;
		float deltaTime = deltaClock.restart().asSeconds();
		float elapsedTime = elapsedClock.getElapsedTime().asSeconds();

		bits += realBitsPerSecond * deltaTime; allBits += realBitsPerSecond * deltaTime;

		updateStar(star, centre, elapsedTime, starScale, allBits);
		starScale += (1.f - starScale) * 0.1f;

		updateStream(window, centre, deltaTime);

		bitsText.setString("-" + format(bits) + " Bits");
		centreText(bitsText, { clickRect.getPosition().x, clickRect.getPosition().y + 400 });

		bitsPerSecondText.setString("-" + format(realBitsPerSecond, true) + " Bits per Second");
		centreText(bitsPerSecondText, { clickRect.getPosition().x, clickRect.getPosition().y + 440 });

		bytesText.setString("-" + format(bytes, true) + " Bytes");
		centreText(bytesText, { clickRect.getPosition().x, clickRect.getPosition().y + 400 });

		sf::RenderStates states;
		states.blendMode = sf::BlendAdd;

		window.clear(sf::Color::Black);

		if (!reinitialisation && !initialisation) {
			window.draw(bitsText);
			window.draw(bitsPerSecondText);

			updateStream(window, centre, deltaTime);
			for (auto& d : dataStream) {
				window.draw(d.bit);
			}

			updateLogicGateUI(window, allBits);
			drawTerminalUI(window, bits, allBits);

			window.draw(star.star, states);

			if (showConfirmPopup) {
				drawConfirmPopup(window, reinitialisation);
			}
		}

		if (reinitialisation) {
			if (currentReinitStep == ReinitState::IDLE) {
				currentReinitStep = ReinitState::VORTEX_EXPANSION;
				canClick = false;
			}
			initTimer += deltaTime;
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

					if (initTimer >= 0.5f) currentReinitStep = ReinitState::VORTEX_SHRINK;
					break;
				}

				case ReinitState::VORTEX_SHRINK: {
					starScale += (0.f - starScale) * 0.15f;
					shakeView.move({ (std::rand() % 10 - 5) / 2.f, (std::rand() % 10 - 5) / 2.f });

					window.setView(shakeView);
					window.draw(star.star, states);

					if (initTimer >= 0.7f) {
						starScale = -10.f;
						currentReinitStep = ReinitState::LOADING_BAR;
					}
					break;
				}

				case ReinitState::LOADING_BAR:
					loadingProgress = (initTimer - 0.7f) / 5.0f;

					if (loadingProgress >= 1.0f) {
						loadingProgress = 1.0f;
						currentReinitStep = ReinitState::ROOTDIR;
					}

					drawLoadingUI(window, loadingProgress);
					break;

				case ReinitState::ROOTDIR:
					window.setView(window.getDefaultView());
					loadingProgress = 0.0f;
					initTimer = 0.0f;
					canClickInit = true;

					drawTreeLines(window);
					drawDirTreeUI(window);

					drawInitButton(window);
					window.draw(bytesText);
					break;
			}
		}

		if (initialisation) {
			if (currentInitStep == InitState::IDLE) {
				currentInitStep = InitState::LOADING_BAR;
			}

			initTimer += deltaTime;
			sf::View shakeView = window.getDefaultView();

			switch (currentInitStep) {
				case InitState::LOADING_BAR: {
					loadingProgress = initTimer / 3.0f;
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

					if (initTimer >= 3.5f) {
						window.setView(window.getDefaultView());
						loadingProgress = 0.0f;
						initTimer = 0.0f;
						canClick = true;

						starScale = 1.f;

						currentInitStep = InitState::IDLE;
						initialisation = false;
					}
					break;
				}
			}
		}

		window.draw(lines);

		window.display();

		while (const std::optional gameEvent = window.pollEvent()) {
			if (gameEvent->is<sf::Event::Closed>()) {
				time_t timeStart = time(nullptr);
				save(timeStart, bits, bytes, allBits, allClickedBits, bitsPerSecond, hotfixMult, timesInitialised, logicGateList, hotfixList, dirTree);
				versionSave(voidVersion);
				window.close();
			}

			if (gameEvent->is<sf::Event::MouseWheelScrolled>()) {
				const auto& wheel = gameEvent->getIf<sf::Event::MouseWheelScrolled>();
				scrollOffset -= wheel->delta * 20.f;
				if (scrollOffset < 0) scrollOffset = 0;
			}

			if (gameEvent->is<sf::Event::MouseButtonPressed>()) {
				const auto& mouseEvent = gameEvent->getIf<sf::Event::MouseButtonPressed>();
				sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
				sf::Vector2f mousePos = window.mapPixelToCoords(mousePixelPos);

				if (mouseEvent->button == sf::Mouse::Button::Left && canClick) {
					if (clickRect.getGlobalBounds().contains(mousePos)) {
						long double bitsClicked = bitsPerClick * clickMultiplier * (1 + realBitsPerSecond * 0.05f);
						starScale = 1.1f; bits += bitsClicked; allBits += bitsClicked; allClickedBits += bitsClicked;
					}

					float startX = 40.f;
					float baseFolder1Y = 105.f;
					float baseFolder2Y = baseFolder1Y + 40.f + (currentDir == Directory::LOGIC_GATES ? (getLogicHeight()) : 0.f);
					float baseFolder3Y = baseFolder2Y + 40.f + (currentDir == Directory::HOTFIXES ? (getHotfixHeight()) : 0.f);

					float folder1Y = baseFolder1Y - scrollOffset;
					float folder2Y = baseFolder2Y - scrollOffset;
					float folder3Y = baseFolder3Y - scrollOffset;

					if (sf::FloatRect({ startX, folder1Y }, { 350.f, 30.f }).contains(mousePos)) {
						currentDir = (currentDir == Directory::LOGIC_GATES) ? Directory::NONE : Directory::LOGIC_GATES;
						scrollOffset = 0;
					}
					else if (sf::FloatRect({ startX, folder2Y }, { 350.f, 30.f }).contains(mousePos)) {
						currentDir = (currentDir == Directory::HOTFIXES) ? Directory::NONE : Directory::HOTFIXES;
						scrollOffset = 0;
					}
					else if (sf::FloatRect({ startX, folder3Y }, { 350.f, 30.f }).contains(mousePos)) {
						currentDir = (currentDir == Directory::REINIT) ? Directory::NONE : Directory::REINIT;
						scrollOffset = 0;
					}

					if (currentDir == Directory::LOGIC_GATES) {
						for (auto& lg : logicGateList) {
							if (lg.ver > 0 || allBits >= lg.baseBits) {
								if (lg.rect.getGlobalBounds().contains(mousePos)) {
									if (bits >= lg.currentBits) {
										bits -= lg.currentBits;
										lg.ver++;
										bitsPerSecond += lg.bps;
										lg.currentBits = lg.baseBits * std::pow(logicGateInflation, lg.ver) * costMult;
									}
								}
							}
						}
					}
					else if (currentDir == Directory::HOTFIXES) {
						for (auto& hf : hotfixList) {
							if (hf.written == 0 && hf.rect.getGlobalBounds().contains(mousePos)) {
								if (bits >= hf.bits) {
									bits -= hf.bits;
									hf.written = 1;
									hotfixMult += hf.bitMult;
								}
							}
						}
					}
					else if (currentDir == Directory::REINIT) {
						sf::FloatRect reinitBatHitbox({ startX + 25.f, folder3Y + 40.f }, { 300.f, 30.f });
						if (reinitBatHitbox.contains(mousePos)) {
							showConfirmPopup = true;
						}
					}
				}

				if (mouseEvent->button == sf::Mouse::Button::Left && canClickInit) {
					for (size_t i = 0; i < dirTree.size(); ++i) {
						auto& patch = dirTree[i];

						sf::Vector2f diff = mousePos - patch.pos;
						float distanceSquared = (diff.x * diff.x) + (diff.y * diff.y);

						if (patch.patched == 0 && !patch.disabled && distanceSquared <= (30.f * 30.f)) {
							bool canBuy = false;

							if (i == 0) {
								canBuy = true;
							}
							else if (i == 1 || i == 2) {
								if (dirTree[0].patched == 1) canBuy = true;
							}
							else if (i == 3 || i == 6) {
								if (dirTree[1].patched == 1) canBuy = true;
							}
							else if (i == 4 || i == 5) {
								if (dirTree[2].patched == 1) canBuy = true;
							}
							else if (i == 7) {
								if (dirTree[3].patched == 1) canBuy = true;
							}
							else if (i == 8) {
								if (dirTree[4].patched == 1) canBuy = true;
							}

							if (canBuy && bytes >= patch.bytes) {
								bytes -= patch.bytes;
								patch.patched = 1;

								if (patch.name == "Patch_0") {
									bitsToBytesRate = 1e-6L;
								}
								else if (patch.name == "Patch_3_1") {
									bitsToBytesRate = 3e-7L;
								}
							}
						}
					}
				}
			}
		}
	}
}