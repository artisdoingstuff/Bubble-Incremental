#include "Misc/GFunctions.hpp"
#include "Misc/GIncludes.hpp"
#include "Misc/GVariables.hpp"

#include "Bits/Format.hpp"
#include "Bits/Offline.hpp"

#include "Effects/Bits.hpp"

#include "LogicGate/Hotfixes.hpp"
#include "LogicGate/LogicGate.hpp"

#include "Re-initialization/Directory.hpp"
#include "Re-initialization/Initialization.hpp"
#include "Re-initialization/Re-initialization.hpp"

#include "UI/Terminal.hpp"
#include "UI/Vortex.hpp"

#include "UserData/Local/Loading.hpp"
#include "UserData/Local/Saving.hpp"

int main() {
	hideConsole();

	sf::RenderWindow gameWindow(sf::VideoMode({ 1920, 1080 }), "Void.INC | " + voidVersion, sf::State::Fullscreen);
	gameWindow.setFramerateLimit(60);
	gameWindow.setIcon(sf::Image("icon.png"));

	float wHeight = gameWindow.getSize().y;
	float wWidth = gameWindow.getSize().x;
	sf::Vector2f center = { wWidth / 2, wHeight / 2 };

	time_t timeEnd = 0;

	sf::Clock deltaClock;
	sf::Clock elapsedClock;

	sf::RectangleShape clickRect;
	clickRect.setSize(sf::Vector2f(256, 256));
	clickRect.setOrigin(sf::Vector2f(clickRect.getSize().x / 2, clickRect.getSize().y / 2));
	clickRect.setPosition(center);
	clickRect.setOutlineColor(sf::Color::Black);
	clickRect.setOutlineThickness(5);
	sf::Vector2f clickAreaSize = clickRect.getSize();

	Vortex vortex;
	vortex.vortex = sf::VertexArray(sf::PrimitiveType::LineStrip, 4000);
	float vortexScale = 1.f;

	sf::VertexArray lines(sf::PrimitiveType::Lines);
	for (int i = 0; i < wHeight; i += 4) {
		lines.append(sf::Vertex{ sf::Vector2f(0.f, (float)i), sf::Color(255, 255, 255, 40) });
		lines.append(sf::Vertex{ sf::Vector2f((float)wWidth, (float)i), sf::Color(255, 255, 255, 40) });
	}

	sf::Text bitsText(jetBrainsMono);
	bitsText.setCharacterSize(36);
	bitsText.setFillColor(sf::Color(243,238,225));

	sf::Text bitsPerSecondText(jetBrainsMono);
	bitsPerSecondText.setCharacterSize(20);
	bitsPerSecondText.setFillColor(sf::Color(243, 238, 225));

	sf::Text bytesText(jetBrainsMono);
	bytesText.setCharacterSize(36);
	bytesText.setFillColor(sf::Color(180, 0, 255));

	initLogicGates();
	initHotfixes();
	initRootTree();
	load(timeEnd, bits, bytes, allBits, allClickedBits, bitsPerSecond, hotfixMult, timesInitialized, logicGateList, hotfixList, rootTree);
	offline(timeEnd, bits, allBits, bitsPerSecond, hotfixMult);
	
	while (gameWindow.isOpen()) {
		if (rootTree[1].patched && patch_1Clock.getElapsedTime().asSeconds() >= 30.f && rootTree[1].disabled == 0) {
			patch_1Mult = 1.5f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (1.7f - 1.5f)));
			patch_1Clock.restart();
		}

		bitsToBytesRate = 1e-8L;
		if (rootTree[0].patched) bitsToBytesRate = 1e-6L;
		if (rootTree[3].patched) bitsToBytesRate = 3e-7L;

		bitMultiplier = 1.0L;
		patch_3_2Mult = 1.0L;
		clickMultiplier = 1.0L;
		if (rootTree[2].patched && rootTree[2].disabled == 0) {
			bitMultiplier *= 3.0L; clickMultiplier *= 2.0L;
		}
		if (rootTree[3].patched) bitMultiplier *= 5.5L;
		if (rootTree[4].patched) {
			long double patch3_2Mult = 1.0L + (bytes * 0.002L);
			patch_3_2Mult = std::min(patch3_2Mult, 100.0L);
		}
		if (rootTree[5].patched) {
			bitMultiplier *= 12.0L; clickMultiplier *= 3.0L;
			for (int i = 0; i < 2; ++i) {
				logicGateList[i].bps = 0.L;
			}
		}
		if (rootTree[6].patched) {
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
		if (rootTree[7].patched) {
			bitMultiplier *= 50.0L; byteMult *= 1.5f;
			rootTree[1].disabled = 1; rootTree[2].disabled = 1;
		}
		if (rootTree[8].patched) {
			bitMultiplier *= 75.0L; clickMultiplier *= 10.0L; costMult *= 0.9f;
		}

		long double realBitsPerSecond = (bitsPerSecond * hotfixMult * bitMultiplier * patch_1Mult) * patch_3_2Mult;
		float deltaTime = deltaClock.restart().asSeconds();
		float elapsedTime = elapsedClock.getElapsedTime().asSeconds();

		bits += realBitsPerSecond * deltaTime; allBits += realBitsPerSecond * deltaTime;

		updateVortex(vortex, center, elapsedTime, vortexScale);
		vortexScale += (1.f - vortexScale) * 0.1f;

		updateStream(gameWindow, center, deltaTime);

		bitsText.setString("-" + format(bits) + " Bits");
		centerText(bitsText, { clickRect.getPosition().x, clickRect.getPosition().y + 400 });

		bitsPerSecondText.setString("-" + format(realBitsPerSecond, true) + " Bits per Second");
		centerText(bitsPerSecondText, { clickRect.getPosition().x, clickRect.getPosition().y + 440 });

		bytesText.setString("-" + format(bytes, true) + " Bytes");
		centerText(bytesText, { clickRect.getPosition().x, clickRect.getPosition().y + 400 });

		sf::RenderStates states;
		states.blendMode = sf::BlendAdd;

		gameWindow.clear(sf::Color::Black);

		if (!reinitialization && !initialization) {
			gameWindow.draw(bitsText);
			gameWindow.draw(bitsPerSecondText);

			for (auto& d : dataStream) {
				gameWindow.draw(d.bit);
			}

			updateLogicGateUI(gameWindow, allBits);
			drawTerminalUI(gameWindow, bits, allBits);

			gameWindow.draw(vortex.vortex, states);

			if (showConfirmPopup) {
				drawConfirmPopup(gameWindow, reinitialization);
			}
		}

		if (reinitialization) {
			if (currentReinitStep == ReinitState::IDLE) {
				currentReinitStep = ReinitState::VORTEX_EXPANSION;
				canClick = false;
			}
			initTimer += deltaTime;
			sf::View shakeView = gameWindow.getDefaultView();

			switch (currentReinitStep) {
				case ReinitState::VORTEX_EXPANSION: {
					vortexScale += (20.f - vortexScale) * 0.05f;

					float intensity = (vortexScale / 20.f) * 15.f;
					float offsetX = (std::rand() % 100 - 50) / 50.f * intensity;
					float offsetY = (std::rand() % 100 - 50) / 50.f * intensity;
					shakeView.move({ offsetX, offsetY });

					gameWindow.setView(shakeView);
					gameWindow.draw(vortex.vortex, states);

					if (initTimer >= 0.5f) currentReinitStep = ReinitState::VORTEX_SHRINK;
					break;
				}

				case ReinitState::VORTEX_SHRINK: {
					vortexScale += (0.f - vortexScale) * 0.15f;
					shakeView.move({ (std::rand() % 10 - 5) / 2.f, (std::rand() % 10 - 5) / 2.f });

					gameWindow.setView(shakeView);
					gameWindow.draw(vortex.vortex, states);

					if (initTimer >= 0.7f) {
						vortexScale = -10.f;
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

					drawLoadingUI(gameWindow, loadingProgress);
					break;

				case ReinitState::ROOTDIR:
					gameWindow.setView(gameWindow.getDefaultView());
					loadingProgress = 0.0f;
					initTimer = 0.0f;
					canClickInit = true;

					drawRootDirectory(gameWindow, bytes);
					gameWindow.draw(bytesText);
					break;
			}
		}

		if (initialization) {
			if (currentInitStep == InitState::IDLE) {
				currentInitStep = InitState::LOADING_BAR;
			}

			initTimer += deltaTime;
			sf::View shakeView = gameWindow.getDefaultView();

			switch (currentInitStep) {
				case InitState::LOADING_BAR: {
					loadingProgress = initTimer / 3.0f;
					drawLoadingUI(gameWindow, loadingProgress);

					if (loadingProgress >= 1.0f) {
						loadingProgress = 1.0f;
						currentInitStep = InitState::VORTEX_EXPANSION;
						vortexScale = 0.1f;
					}
					break;
				}

				case InitState::VORTEX_EXPANSION: {
					vortexScale += (1.f - vortexScale) * 0.1f;

					float intensity = (1.f - vortexScale) * 20.f;
					float offsetX = (std::rand() % 100 - 50) / 50.f * intensity;
					float offsetY = (std::rand() % 100 - 50) / 50.f * intensity;
					shakeView.move({ offsetX, offsetY });
					gameWindow.setView(shakeView);

					gameWindow.draw(vortex.vortex, states);

					for (auto it = dataStream.begin(); it != dataStream.end();) {
						it->pos += it->vel * deltaTime;
						it->bit.setPosition(it->pos);

						gameWindow.draw(it->bit);

						if (it->pos.x < 0 || it->pos.x > wWidth || it->pos.y < 0 || it->pos.y > wHeight) {
							it = dataStream.erase(it);
						}
						else {
							++it;
						}
					}

					if (initTimer >= 3.5f) {
						gameWindow.setView(gameWindow.getDefaultView());
						loadingProgress = 0.0f;
						initTimer = 0.0f;
						canClick = true;

						vortexScale = 1.f;

						currentInitStep = InitState::IDLE;
						initialization = false;
					}
					break;
				}
			}
		}

		gameWindow.draw(lines);

		gameWindow.display();

		while (const std::optional gameEvent = gameWindow.pollEvent()) {
			if (gameEvent->is<sf::Event::Closed>()) {
				time_t timeStart = time(nullptr);
				save(timeStart, bits, bytes, allBits, allClickedBits, bitsPerSecond, hotfixMult, timesInitialized, logicGateList, hotfixList, rootTree);
				gameWindow.close();
			}

			if (gameEvent->is<sf::Event::MouseWheelScrolled>()) {
				const auto& wheel = gameEvent->getIf<sf::Event::MouseWheelScrolled>();
				scrollOffset -= wheel->delta * 20.f;
				if (scrollOffset < 0) scrollOffset = 0;
			}

			if (gameEvent->is<sf::Event::MouseButtonPressed>()) {
				const auto& mouseEvent = gameEvent->getIf<sf::Event::MouseButtonPressed>();
				sf::Vector2i mousePixelPos = sf::Mouse::getPosition(gameWindow);
				sf::Vector2f mousePos = gameWindow.mapPixelToCoords(mousePixelPos);

				if (mouseEvent->button == sf::Mouse::Button::Left && canClick) {
					if (clickRect.getGlobalBounds().contains(mousePos)) {
						long double bitsClicked = bitsPerClick * clickMultiplier * (1 + realBitsPerSecond * 0.05f);
						vortexScale = 1.1f; bits += bitsClicked; allBits += bitsClicked; allClickedBits += bitsClicked;
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
					for (size_t i = 0; i < rootTree.size(); ++i) {
						auto& patch = rootTree[i];
						if (patch.patched == 0 && patch.rect.getGlobalBounds().contains(mousePos)) {
							bool canBuy = false;
							if (i == 0) canBuy = true;
							else if (i < 3 && rootTree[i - 1].patched == 1) canBuy = true;
							else if (i >= 3 && rootTree[2].patched == 1) canBuy = true;

							if (canBuy && bytes >= patch.bytes) {
								bytes -= patch.bytes;
								patch.patched = 1;

								if (patch.name == "Patch_0") {
									bitsToBytesRate = 1e-6L;
								}
								if (patch.name == "Patch_3_1") {
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