#pragma once

#include "../../Misc/Globals/GIncludes.hpp"
#include "../../Misc/Lore.hpp"
#include "Download.hpp"

inline int hotfixPage = 0;
const int HF_PER_PAGE = 40;

inline void backgroundDeco(sf::RenderWindow& window, sf::Vector2f pos, sf::Vector2f size, bool buy) {
    sf::Color traceColor = buy ? sf::Color(243, 238, 225, 50) : sf::Color(140, 140, 140, 30);

    sf::VertexArray traces(sf::PrimitiveType::Lines);

    auto addVertex = [&](sf::Vector2f vPos, sf::Color vColor) {
        traces.append(sf::Vertex{ vPos, vColor });
        };

    float l = 15.f;

    addVertex(pos, traceColor);
    addVertex(pos + sf::Vector2f(l, 0.f), traceColor);
    addVertex(pos, traceColor);
    addVertex(pos + sf::Vector2f(0.f, l), traceColor);

    addVertex(pos + size, traceColor);
    addVertex(pos + size - sf::Vector2f(l, 0.f), traceColor);
    addVertex(pos + size, traceColor);
    addVertex(pos + size - sf::Vector2f(0.f, l), traceColor);

    sf::Vector2f traceStart = pos + sf::Vector2f(size.x * 0.75f, 0.f);

    addVertex(traceStart, traceColor);
    addVertex(traceStart + sf::Vector2f(35.f, 20.f), traceColor);

    addVertex(traceStart + sf::Vector2f(35.f, 20.f), traceColor);
    addVertex(traceStart + sf::Vector2f(35.f, size.y), traceColor);

    window.draw(traces);

    sf::CircleShape pad(2.f);
    pad.setFillColor(traceColor);

    for (int i = 0; i < 3; ++i) {
        pad.setPosition(pos + sf::Vector2f(-1.f, 20.f + (i * 15.f)));
        window.draw(pad);
    }
}

inline void drawTerminalModule(sf::RenderWindow& window, std::string title, float animation, std::function<void(sf::Vector2f)> contentDraw) {
    if (animation <= 0.f) return;

    sf::Vector2u winSize = window.getSize();
    float width = winSize.x * 0.8f;
    float height = winSize.y * 0.7f;

    float targetY = (winSize.y - height) / 2.f;
    float currentY = winSize.y - (height * animation) - targetY;

    sf::RectangleShape overlay({ (float)winSize.x, (float)winSize.y });
    overlay.setFillColor(sf::Color(0, 0, 0, static_cast<std::uint8_t>(180 * animation)));
    window.draw(overlay);

    sf::RectangleShape frame({ width, height });
    frame.setPosition({ (winSize.x - width) / 2.f, currentY });
    frame.setFillColor(sf::Color(10, 10, 10));
    frame.setOutlineColor(sf::Color(50, 50, 50, static_cast<std::uint8_t>(255 * animation)));
    frame.setOutlineThickness(1.f);
    window.draw(frame);

    sf::RectangleShape titleBar({ width, 30.f });
    titleBar.setPosition(frame.getPosition());
    titleBar.setFillColor(sf::Color(40, 40, 40));
    window.draw(titleBar);

    sf::Text t(jetBrainsMono, "> " + title, 14);
    t.setPosition(titleBar.getPosition() + sf::Vector2f(10.f, 5.f));
    t.setFillColor(sf::Color(243, 238, 225));
    window.draw(t);

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    sf::Text c(jetBrainsMono, "- X", 14);
    c.setPosition({ titleBar.getPosition().x + (width * 0.97f), titleBar.getPosition().y + 5.f});
    c.setFillColor(sf::Color(243, 238, 225));
    bool canClose = c.getGlobalBounds().contains(mousePos);
    window.draw(c);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) || sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && canClose) activeTab = Tab::NONE;

    contentDraw(frame.getPosition() + sf::Vector2f(20.f, 50.f));
}

inline void drawTerminalUI(sf::RenderWindow& window, long double& bits, long double allBits, float dt) {
    sf::Vector2u winSize = window.getSize();
    float moduleWidth = winSize.x * 0.8f;
    float moduleHeight = winSize.y * 0.7f;

    if (activeTab != Tab::NONE) tabProgress = std::min(1.f, tabProgress + dt * 5.f);
    else tabProgress = std::max(0.f, tabProgress - dt * 5.f);

    auto drawTabButton = [&](std::string label, float x, Tab type) {
        sf::RectangleShape btn({ 180.f, 40.f });
        btn.setPosition({ x, winSize.y - 50.f });
        btn.setFillColor(activeTab == type ? sf::Color(40, 40, 40, 150) : sf::Color(15, 15, 15, 200));
        btn.setOutlineColor(sf::Color(50, 50, 50, 100));
        btn.setOutlineThickness(1.f);
        window.draw(btn);

        sf::Text txt(jetBrainsMono, label, 13);
        txt.setPosition(btn.getPosition() + sf::Vector2f(15.f, 10.f));
        window.draw(txt);
    };

    if (!reinitialisation) drawTabButton("logic.bat", 10.f, Tab::LOGIC);
    if (!reinitialisation) drawTabButton("hotfixes.bat", 200.f, Tab::HOTFIX);
    if (!reinitialisation) drawTabButton("stats.bat", 390.f, Tab::STATS);
    if (!reinitialisation && allBits >= 5000000.0L) drawTabButton("reinit.bat", 580.f, Tab::REINIT);
	if (!reinitialisation && allBits >= 1e45L) drawTabButton("logs.bat", 770.f, Tab::LOGS);

    if (reinitialisation) drawTabButton("init.bat", 10.f, Tab::INIT);

    if (activeTab == Tab::LOGIC) {
        drawTerminalModule(window, "void://hardware/logic.bat" + getCursor(), tabProgress, [&](sf::Vector2f start) {
            float boxW = (moduleWidth - 100.f) / 5.f;
            float boxH = (moduleHeight - 120.f) / 4.f;
            float pad = 10.f;
            float btnX = 0.f;
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            for (size_t i = 0; i < logicGateList.size(); ++i) {
                if (i >= 20) break;
                auto& lg = logicGateList[i];

                int col = i % 5;
                int row = i / 5;
                sf::Vector2f pos = start + sf::Vector2f(col * (boxW + pad), row * (boxH + pad));

                lg.rect.setSize({ boxW, boxH });
                lg.rect.setPosition(pos);
                lg.rect.setFillColor(sf::Color::Black);
                lg.rect.setOutlineColor(sf::Color(50, 50, 50));
                lg.rect.setOutlineThickness(1.f);
                window.draw(lg.rect);

                backgroundDeco(window, pos, { boxW, boxH }, (bits >= lg.currentBits));

                for (int p = 0; p < 4; ++p) {
                    sf::RectangleShape pin({ 4.f, 2.f });
                    pin.setFillColor(sf::Color(243, 238, 225, 100));
                    pin.setPosition({ pos.x - 2.f, pos.y + 15.f + (p * 15.f) });
                    window.draw(pin);
                    pin.setPosition({ pos.x + boxW - 2.f, pos.y + 15.f + (p * 15.f) });
                    window.draw(pin);
                }

                sf::Text n(jetBrainsMono, lg.name + "_" + std::to_string(lg.ver) + ".bin", 13);
                n.setPosition(pos + sf::Vector2f(12.f, 10.f));
                n.setFillColor(sf::Color(243, 238, 225));

                sf::Text d(jetBrainsMono, lg.desc, 10);
                d.setPosition(pos + sf::Vector2f(12.f, 32.f));
                d.setFillColor(sf::Color(140, 140, 140));

                auto [totalCost, amount] = getDownload(lg, bits, currentBuy);

                sf::Text c(jetBrainsMono, "-" + format(totalCost) + "_bits.tmp - x" + std::to_string(amount), 11);
                c.setPosition(pos + sf::Vector2f(12.f, boxH - 22.f));
                c.setFillColor(bits >= totalCost ? sf::Color(243, 238, 225) : sf::Color(140, 140, 140));
                window.draw(c);

                window.draw(n);
                window.draw(d);
                window.draw(c);
            }

            std::vector<std::pair<std::string, Download>> modes = {
                {"x1", Download::X1}, {"x5", Download::X5}, {"x10", Download::X10},
                {"x50", Download::X50}, {"x100", Download::X100}, {"MAX", Download::MAX}
            };

            for (auto& m : modes) {
                sf::RectangleShape b({ 50.f, 25.f });
                b.setPosition(start + sf::Vector2f(btnX, moduleHeight - 80.f));
                bool hovered = b.getGlobalBounds().contains(mousePos);

                b.setFillColor(currentBuy == m.second ? sf::Color(30, 30, 30) : sf::Color::Black);
                b.setOutlineColor(sf::Color(50, 50, 50));
                b.setOutlineThickness(1.f);

                sf::Text bt(jetBrainsMono, m.first, 11);
                bt.setPosition(b.getPosition() + sf::Vector2f(10, 5));

                window.draw(b);
                window.draw(bt);

                if (hovered && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && cooldown.getElapsedTime().asMilliseconds() > 200) {
                    currentBuy = m.second;
                    cooldown.restart();
                }
                btnX += 55.f;
            }
            });
    }

    else if (activeTab == Tab::HOTFIX) {
        drawTerminalModule(window, "void://hardware/hotfixes.bat" + getCursor(), tabProgress, [&](sf::Vector2f start) {
            float size = (moduleWidth - 140.f) / 10.f;
            float pad = 10.f;
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            size_t startIdx = hotfixPage * HF_PER_PAGE;
            size_t endIdx = std::min(startIdx + HF_PER_PAGE, hotfixList.size());

            for (size_t i = startIdx; i < endIdx; ++i) {
                auto& hf = hotfixList[i];
                int localIdx = static_cast<int>(i - startIdx);
                sf::Vector2f pos = start + sf::Vector2f((localIdx % 10) * (size + pad), (localIdx / 10) * (size + pad));

                hf.rect.setSize({ size, size });
                hf.rect.setPosition(pos);
                hf.rect.setFillColor(sf::Color::Black);
                hf.rect.setOutlineColor(hf.written ? sf::Color(60, 60, 60) : sf::Color(100, 100, 100));
                hf.rect.setOutlineThickness(1.f);
                window.draw(hf.rect);

                backgroundDeco(window, pos, { size, size }, (bits >= hf.bits));

                sf::Text n(jetBrainsMono, hf.name + (hf.written ? ".bin" : ".pkg"), 10);
                n.setPosition(pos + sf::Vector2f(6.f, 6.f));
                window.draw(n);

                sf::Text s(jetBrainsMono, hf.written ? "[ LOADED ]" : "-" + format(hf.bits) + "_bits.tmp", 9);
                s.setPosition(pos + sf::Vector2f(6.f, 20.f));
                s.setFillColor(!hf.written && bits < hf.bits ? sf::Color(140, 140, 140) : sf::Color(243, 238, 225));
                window.draw(s);
            }

            auto drawNav = [&](const std::string& str, sf::Vector2f offset, bool isPrev) {
                sf::Text t(jetBrainsMono, str, 14);
                t.setPosition(start + sf::Vector2f(moduleWidth / 2.f + offset.x, moduleHeight - 80.f));
                bool hovered = t.getGlobalBounds().contains(mousePos);
                t.setFillColor(hovered ? sf::Color(243, 238, 225) : sf::Color(100, 100, 100));
                window.draw(t);

                if (hovered && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && cooldown.getElapsedTime().asMilliseconds() > 200) {
                    if (isPrev && hotfixPage > 0) { hotfixPage--; cooldown.restart(); }
                    if (!isPrev && (hotfixPage + 1) * HF_PER_PAGE < (int)hotfixList.size()) { hotfixPage++; cooldown.restart(); }
                }
                };

            drawNav("<< ", { -45.f, 0.f }, true);
            drawNav(" >>", { 20.f, 0.f }, false);

            sf::Text pgNum(jetBrainsMono, "PAGE " + std::to_string(hotfixPage + 1), 14);
            pgNum.setOrigin({ pgNum.getLocalBounds().size.x / 2.f, 0.f });
            pgNum.setPosition(start + sf::Vector2f(moduleWidth / 2.f, moduleHeight - 80.f));
            window.draw(pgNum);

            int affordableCount = 0;
            for (const auto& hf : hotfixList) {
                if (!hf.written && bits >= hf.bits) affordableCount++;
            }

            if (affordableCount > 0) {
                sf::Vector2f bPos = start + sf::Vector2f(0.f, moduleHeight - 80.f);
                sf::RectangleShape bBtn({ 180.f, 30.f });
                bBtn.setPosition(bPos - sf::Vector2f(0.f, 20.f));
                bool bHover = bBtn.getGlobalBounds().contains(mousePos);

                bBtn.setFillColor(bHover ? sf::Color(30, 30, 30) : sf::Color::Black);
                bBtn.setOutlineColor(sf::Color(50, 50, 50));
                bBtn.setOutlineThickness(1.f);
                window.draw(bBtn);

                sf::Text bTxt(jetBrainsMono, "install_hotfixes.bat", 12);
                bTxt.setPosition(bPos + sf::Vector2f(pad + 1.f, -15.f));
                bTxt.setFillColor(sf::Color(243, 238, 225));
                window.draw(bTxt);

                if (bHover && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && cooldown.getElapsedTime().asMilliseconds() > 200) {
                    for (auto& hf : hotfixList) {
                        if (!hf.written && bits >= hf.bits) {
                            bits -= hf.bits;
                            hf.written = 1;
                            hotfixMult += hf.bitMult;
                        }
                    }
                    cooldown.restart();
                }
            }
            });
    }

    else if (activeTab == Tab::STATS) {
        drawTerminalModule(window, "void://hardware/stats.bat" + getCursor(), tabProgress, [&](sf::Vector2f start) {
            sf::Text s(jetBrainsMono,
                format(allBits) + "_all_bits.tmp\n\n" +
                format(allClickedBits) + "_clicked_bits.tmp\n\n" +
                std::to_string(timesInitialised) + "_times_init.tmp\n\n" +
                voidVersion + "\n\n" +
                "U-01-A.bin",
                14);
            s.setPosition(start);
            s.setFillColor(sf::Color(243, 238, 225));
            window.draw(s);

            sf::Text c(jetBrainsMono, "More coming\nsoon...", 56);
            c.setPosition(start + sf::Vector2f(moduleWidth * 0.7f, moduleHeight / 2.f - 100.f));
            c.setFillColor(sf::Color(100, 100, 100));
            window.draw(c);
            });
    }

    else if (activeTab == Tab::LOGS) {
        drawTerminalModule(window, "void://restricted/logs.bat" + getCursor(), tabProgress, [&](sf::Vector2f start) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (!logsUnlocked) {
                sf::Text t(jetBrainsMono, "OMEGA-LEVEL ACCESS CODE REQUIRED:\n> " + logInput + getCursor(), 56);
				t.setOrigin({ t.getGlobalBounds().size.x / 2.f, 0.f });
                t.setPosition(start + sf::Vector2f(moduleWidth / 2.f, moduleHeight * 0.35f));
                t.setFillColor(sf::Color(243, 238, 225));
                window.draw(t);

                if (logInput == "050126") {
                    logsUnlocked = true;
                    logInput = "";
                }
            }
            else {
                float sidebarWidth = 200.f;

                for (size_t i = 0; i < loreEntries.size(); ++i) {
                    sf::Text logTitle(jetBrainsMono, "> " + loreEntries[i].title, 13);
                    logTitle.setPosition(start + sf::Vector2f(10.f, 20.f + (i * 25.f)));

                    bool isHovered = sf::FloatRect(logTitle.getPosition(), { sidebarWidth, 20.f }).contains(mousePos);

                    if (selectedLog == i) logTitle.setFillColor(sf::Color(243, 238, 225));
                    else if (isHovered) logTitle.setFillColor(sf::Color::White);
                    else logTitle.setFillColor(sf::Color(100, 100, 100));

                    if (isHovered && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                        selectedLog = i;
                    }
                    window.draw(logTitle);
                }

                sf::RectangleShape line({ 1.f, moduleHeight - 100.f });
                line.setPosition(start + sf::Vector2f(sidebarWidth, 10.f));
                line.setFillColor(sf::Color(50, 50, 50));
                window.draw(line);

                auto& entry = loreEntries[selectedLog];

                sf::Text h(jetBrainsMono, entry.title + " - " + entry.cycle, 14);
                h.setPosition(start + sf::Vector2f(sidebarWidth + 20.f, 20.f));
                h.setFillColor(sf::Color(243, 238, 225));
                window.draw(h);

                sf::Text b(jetBrainsMono, entry.content, 12);
                b.setPosition(start + sf::Vector2f(sidebarWidth + 20.f, 50.f));
                b.setFillColor(sf::Color(180, 180, 180));
                window.draw(b);
            }
            });
    }
}