#pragma once

#include "../../Misc/Globals/GIncludes.hpp"
#include "../../Misc/Globals/GVariables.hpp"

inline void drawOfflineUI(sf::RenderWindow& window, sf::Vector2f& centre) {
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	sf::Vector2f boxSize(600.f, 200.f);

	sf::RectangleShape overlay(sf::Vector2f(window.getSize()));
	overlay.setFillColor(sf::Color(0, 0, 0, 180));
	window.draw(overlay);

    sf::RectangleShape box(boxSize);
    box.setOrigin(boxSize / 2.f);
    box.setPosition(centre);
    box.setFillColor(sf::Color(10, 10, 10));
    box.setOutlineColor(sf::Color(50, 50, 50));
    box.setOutlineThickness(1);

    sf::RectangleShape titleBar({ boxSize.x, 30.f });
    titleBar.setOrigin({ boxSize.x / 2.f, 0.f });
    titleBar.setPosition({ centre.x, centre.y - (boxSize.y / 2.f) });
    titleBar.setFillColor(sf::Color(40, 40, 40));

    sf::Text t(jetBrainsMono, "> void://bits/offline.bat" + getCursor(), 14);
    t.setPosition({ titleBar.getPosition().x - (boxSize.x / 2.f) + 10.f, titleBar.getPosition().y + 5.f });
    t.setFillColor(sf::Color(243, 238, 225));

    sf::Text c(jetBrainsMono, "- X", 14);
    c.setPosition({ titleBar.getPosition().x + ((boxSize.x / 2.f) * 0.85f), titleBar.getPosition().y + 5.f });
    c.setFillColor(sf::Color(243, 238, 225));

    window.draw(box);
    window.draw(titleBar);
    window.draw(t);
    window.draw(c);

	std::string offlineMsg =
		"Connection restored. Analysis of bits harvested:\n"
		"The Star consumed -" + format(accOfflineBits) + " bits during your inactivity.\n"
		"The Void corrupted -" + format(accOfflineBits * (1.f - offlineMultiplier)) + " into -" + format(accOfflineMalbits) + " malbits.";

	sf::Text o(jetBrainsMono, offlineMsg, 16);
	o.setOrigin({ o.getLocalBounds().size.x / 2.f, o.getLocalBounds().size.y / 2.f - 10.f });
	o.setPosition(centre);
    o.setFillColor(sf::Color(200, 200, 200));
	window.draw(o);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) || sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && c.getGlobalBounds().contains(mousePos)) {
        showOffline = false; playSFX("button");
    }
}