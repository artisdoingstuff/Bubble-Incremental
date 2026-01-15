#pragma once

#include "../Misc/GIncludes.hpp"

inline void positionTreeNodes(sf::Vector2u windowSize) {
    if (dirTree.size() < 9) return;

    sf::Vector2f startPos(windowSize.x / 4.f, windowSize.y / 2.f);
    float horizontalGap = 180.f;
    float verticalBranch = 120.f;

    dirTree[0].pos = startPos;
    dirTree[1].pos = dirTree[0].pos + sf::Vector2f(horizontalGap, 0.f);
    dirTree[2].pos = dirTree[1].pos + sf::Vector2f(horizontalGap, 0.f);

	dirTree[3].pos = dirTree[2].pos + sf::Vector2f(horizontalGap, -verticalBranch); // Path A (Top)
	dirTree[4].pos = dirTree[2].pos + sf::Vector2f(horizontalGap, verticalBranch); // Path B (Bottom)

    // Path A Continuation
    dirTree[5].pos = dirTree[3].pos + sf::Vector2f(horizontalGap, 0.f); // 4_1
    dirTree[7].pos = dirTree[5].pos + sf::Vector2f(horizontalGap, 0.f); // 5_1

    // Path B Continuation
    dirTree[6].pos = dirTree[4].pos + sf::Vector2f(horizontalGap, 0.f); // 4_2
    dirTree[8].pos = dirTree[6].pos + sf::Vector2f(horizontalGap, 0.f); // 5_2

    for (auto& node : dirTree) {
        node.nodeCircle.setPosition(node.pos);
    }
}

inline void drawTreeLines(sf::RenderWindow& window) {
    if (dirTree.size() < 9) return;

    auto drawCable = [&](int parentIdx, int childIdx) {
        if (!dirTree[parentIdx].patched) return;

        sf::Vector2f start = dirTree[parentIdx].pos;
        sf::Vector2f end = dirTree[childIdx].pos;

        sf::Vector2f direction = end - start;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        float angle = std::atan2(direction.y, direction.x) * 180.f / pi;

        sf::RectangleShape cable({ distance, 3.f });
        cable.setOrigin({ 0.f, 1.5f });
        cable.setPosition(start);
        cable.setRotation(sf::degrees(angle));

        if (dirTree[childIdx].patched) {
            cable.setFillColor(sf::Color(0, 255, 150, 200));
        }
        else {
            cable.setFillColor(sf::Color(0, 255, 150, 50));
        }

        window.draw(cable);
    };

    drawCable(0, 1);
    drawCable(1, 2);

    // Split
    drawCable(2, 3); // 2 to 3_1
    drawCable(2, 4); // 2 to 3_2

    // Path A (Top)
    drawCable(3, 5); // 3_1 to 4_1
    drawCable(5, 7); // 4_1 to 5_1

    // Path B (Bottom)
    drawCable(4, 6); // 3_2 to 4_2
    drawCable(6, 8); // 4_2 to 5_2
}

inline void drawDirTreeUI(sf::RenderWindow& window) {
    drawTreeLines(window);

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    int hoveredIdx = -1;

    auto isVisible = [&](int idx) -> bool {
        if (idx == 0) return true;

        if (idx == 1) return dirTree[0].patched;
        if (idx == 2) return dirTree[1].patched;
        if (idx == 3 || idx == 4) return dirTree[2].patched;
        if (idx == 5) return dirTree[3].patched;
        if (idx == 7) return dirTree[5].patched;
        if (idx == 6) return dirTree[4].patched;
        if (idx == 8) return dirTree[6].patched;
        return false;
        };

    for (int i = 0; i < dirTree.size(); ++i) {
        std::string basic = dirTree[i].name + ".sys";
        std::string status;

        if (!isVisible(i)) continue;

        sf::Vector2f diff = mousePos - dirTree[i].pos;
        if ((diff.x * diff.x + diff.y * diff.y) < (30.f * 30.f)) {
            hoveredIdx = i;
            dirTree[i].nodeCircle.setOutlineThickness(4.f);
        }
        else {
            dirTree[i].nodeCircle.setOutlineThickness(2.f);
        }

        if (isVisible(i)) {
            if (dirTree[i].disabled) {
                dirTree[i].nodeCircle.setFillColor(sf::Color(20, 20, 20));
                dirTree[i].nodeCircle.setOutlineColor(sf::Color(100, 100, 100));
                status = "[ OFFLINE ]";
                
            }
            else if (dirTree[i].patched) {
                dirTree[i].nodeCircle.setFillColor(sf::Color(5, 10, 5));
                dirTree[i].nodeCircle.setOutlineColor(sf::Color(0, 255, 150));
                status = "[ ONLINE ]";
            }
            else {
                dirTree[i].nodeCircle.setFillColor(sf::Color(0, 0, 0));
                dirTree[i].nodeCircle.setOutlineColor(sf::Color(50, 100, 75));
                status = "-" + format(dirTree[i].bytes, true) + " Bytes";
            }

            window.draw(dirTree[i].nodeCircle);

            sf::Text label(jetBrainsMono, basic, 12);
            label.setOrigin({ label.getLocalBounds().size.x / 2.f, 0.f });
            label.setPosition(dirTree[i].pos + sf::Vector2f(0.f, 35.f));
            window.draw(label);

		    sf::Text statusText(jetBrainsMono, status, 12);
		    statusText.setOrigin({ statusText.getLocalBounds().size.x / 2.f, 0.f });
		    statusText.setPosition(dirTree[i].pos + sf::Vector2f(0.f, 50.f));
		    window.draw(statusText);
        }
    }

    if (hoveredIdx != -1) {
        sf::Text desc(jetBrainsMono, dirTree[hoveredIdx].desc, 13);
        desc.setFillColor(sf::Color::White);

        sf::FloatRect textBounds = desc.getLocalBounds();
        sf::RectangleShape tooltBox({ textBounds.size.x + 20.f, textBounds.size.y + 25.f });
        tooltBox.setFillColor(sf::Color(5, 10, 5, 230));
        tooltBox.setOutlineColor(sf::Color(100, 100, 100));
        tooltBox.setOutlineThickness(1.f);

        sf::Vector2f tooltPos = mousePos + sf::Vector2f(15.f, 15.f);

        if (tooltPos.x + tooltBox.getSize().x > window.getSize().x) {
            tooltPos.x -= (tooltBox.getSize().x + 20.f);
        }

        tooltBox.setPosition(tooltPos);
        desc.setPosition(tooltPos + sf::Vector2f(10.f, 5.f));

        window.draw(tooltBox);
        window.draw(desc);
    }
}