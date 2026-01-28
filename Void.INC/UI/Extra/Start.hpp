#pragma once

#include "../../Misc/Globals/GIncludes.hpp"

inline void drawStartUI(sf::RenderWindow& window, sf::RenderStates& states, Star& star, float et, float dt, sf::Vector2f& pos) {
	const sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	sf::Text t(jetBrainsMono, "Void.INC", 124);
	t.setPosition({ 80, window.getSize().y * 0.05f });
	t.setFillColor(sf::Color(243, 238, 225));

	sf::Text v(jetBrainsMono, voidVersion, 28);
	v.setPosition({ 120, window.getSize().y * 0.15f + 20.f });
	v.setFillColor(sf::Color(140, 140, 140));

	sf::Text e(jetBrainsMono, "Enter", 48);
	e.setPosition({ 80, window.getSize().y * 0.5f });
	bool pHover = e.getGlobalBounds().contains(mousePos);
	e.setFillColor(pHover ? sf::Color(200, 200, 30) : sf::Color(243, 238, 225));

	sf::Text s(jetBrainsMono, "Options", 48);
	s.setPosition({ 80, window.getSize().y * 0.6f });
	s.setFillColor(sf::Color(140, 140, 140));

	sf::Text q(jetBrainsMono, "Quit", 48);
	q.setPosition({ 80, window.getSize().y * 0.7f });
	bool qHover = q.getGlobalBounds().contains(mousePos);
	q.setFillColor(qHover ? sf::Color(200, 200, 30) : sf::Color(243, 238, 225));

	updateStar(star, pos, et, 1.f, allBits);
	updateStream(window, pos, dt, 2);
	window.draw(star.star, states);
	for (auto& d : dataStream) {
		window.draw(d.bit);
	}

	if (!start) {
		window.draw(t);
		window.draw(v);
		window.draw(e);
		window.draw(s);
		window.draw(q);
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && cooldown.getElapsedTime().asMilliseconds() > 300) {
		if (pHover && canClickStart) {
			start = true;
		}
		if (qHover && canClickStart) {
			window.close();
		}
		cooldown.restart();
	}
}