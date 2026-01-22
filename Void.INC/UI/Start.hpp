#pragma once

#include "../Misc/GIncludes.hpp"

inline void drawStartUI(sf::RenderWindow& window) {
	const sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	sf::Vector2f pos = { window.getSize().x * 0.7f, window.getSize().y / 2.f };

	sf::Text t(jetBrainsMono, "Void.INC", 124);
	t.setPosition({ 80, window.getSize().y * 0.05f });
	t.setFillColor(sf::Color(243, 238, 225));

	sf::Text v(jetBrainsMono, voidVersion, 28);
	v.setPosition({ 120, window.getSize().y * 0.15f + 20.f });
	v.setFillColor(sf::Color(140, 140, 140));

	sf::Text p(jetBrainsMono, "Play", 48);
	p.setPosition({ 80, window.getSize().y * 0.5f });
	bool pHover = p.getGlobalBounds().contains(mousePos);
	p.setFillColor(pHover ? sf::Color(200, 200, 30) : sf::Color(243, 238, 225));

	sf::Text s(jetBrainsMono, "Settings", 48);
	s.setPosition({ 80, window.getSize().y * 0.6f });
	s.setFillColor(sf::Color(140, 140, 140));

	sf::Text q(jetBrainsMono, "Quit", 48);
	q.setPosition({ 80, window.getSize().y * 0.7f });
	bool qHover = q.getGlobalBounds().contains(mousePos);
	q.setFillColor(qHover ? sf::Color(200, 200, 30) : sf::Color(243, 238, 225));

	sf::RenderStates states;
	states.blendMode = sf::BlendAdd;

	updateStar(star, pos, elapsedTime, starScale, allBits);
	updateStream(window, pos, deltaTime);
	window.draw(star.star, states);
	for (auto& d : dataStream) {
		window.draw(d.bit);
	}

	window.draw(t);
	window.draw(v);
	window.draw(p);
	window.draw(s);
	window.draw(q);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && cooldown.getElapsedTime().asMilliseconds() > 300) {
		if (pHover && canClickStart) {
			showStart = false;
			canClickStart = false;
			canClick = true;
		}
		if (qHover && canClickStart) {
			window.close();
		}
		cooldown.restart();
	}
}