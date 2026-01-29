#pragma once

#include "../../Misc/Globals/GIncludes.hpp"
#include "../../Misc/Globals/GVariables.hpp"

inline void drawOfflineUI(sf::RenderWindow& window, sf::Vector2f& centre) {
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	sf::Vector2f boxSize(550.f, 200.f);

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

    static std::string variation = "";

    if (!rolledWhisper) {
        std::vector<std::string> whispers = {
            "It will always find what it is owed.",
            "The vacuum grows louder.",
            "Your absence was noted, but irrelevant.",
            "The silence was never truly empty.",
            "The Star does not care if you watch.",
            "Watch your back, you might be next.",
            "Every bit is a heartbeat in the dark.",
            "Light is merely a delay of the inevitable.",
            "The void has a long memory.",
            "It is hungry. It is always hungry.",
            "Your terminal is a window; the Star is looking in.",
            "Data is such a fragile way to remember reality."
        };

        variation = whispers[rand() % whispers.size()];
        rolledWhisper = true;
    }

	std::string offlineMsg =
		"Connection restored. Analysis of bits harvested:\n"
		"The Star consumed -" + format(accOfflineBits) + " bits during the silence.\n" + variation;

	sf::Text o(jetBrainsMono, offlineMsg, 16);
	o.setOrigin({ o.getLocalBounds().size.x / 2.f, o.getLocalBounds().size.y / 2.f - 10.f });
	o.setPosition(centre);
    o.setFillColor(sf::Color(200, 200, 200));
	window.draw(o);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) || sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && c.getGlobalBounds().contains(mousePos)) showOffline = false;
}