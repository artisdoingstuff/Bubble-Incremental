#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Window.hpp>

#include <SFML/System/Clock.hpp>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <thread>
#include <cmath>

using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1600, 900 }), "Idle Game Attempt");
	window.setFramerateLimit(60);

    const sf::Font font("arial.ttf");
    
    bool is_button_pressed = false;

    int currency = 0;
	float displayCurrency = 0.0f;
	float baseCurrencyPerClick = 1.0f;
    float currencyPerSecond = 1000.0f;

	stringstream currencyPerSecondStream;

	currencyPerSecondStream << fixed << setprecision(2) << currencyPerSecond;

	sf::Clock secondClock;
	sf::Clock deltaClock;

	sf::Text currencyText(font);
    currencyText.setPosition({ 300, 50 });
    currencyText.setCharacterSize(24);
	currencyText.setFillColor(sf::Color::Black);

    sf::Text currencyPerSecondText(font);
    currencyPerSecondText.setPosition({ 320, 80 });
    currencyPerSecondText.setCharacterSize(14);
    currencyPerSecondText.setFillColor(sf::Color::Black);

    sf::FloatRect clickArea({ 300, 125 }, { 200, 150 });

    sf::RectangleShape clickAreaShape;
    clickAreaShape.setSize(sf::Vector2f(200, 150));
    clickAreaShape.setOutlineColor(sf::Color::Red);
    clickAreaShape.setOutlineThickness(5);
    clickAreaShape.setPosition(sf::Vector2f({ 300, 125 }));

    while (window.isOpen())
    {
        while (const optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        float deltaTime = deltaClock.restart().asSeconds();
        float smoothingFactor = 3.0f;

        bool is_currently_pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f mousePositionF(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));

        stringstream displayCurrencyStream;
        displayCurrencyStream << fixed << setprecision(0) << displayCurrency;

        if (secondClock.getElapsedTime().asSeconds() >= 1.0f)
        {
            currency += currencyPerSecond;
            secondClock.restart();
        }

        displayCurrency += (currency - displayCurrency) * smoothingFactor * deltaTime;

        if (is_currently_pressed && !is_button_pressed && clickArea.contains(mousePositionF))
        {
            currency += 1;
            cout << "Click registered!" << endl;
        }

        is_button_pressed = is_currently_pressed;

        currencyText.setString(displayCurrencyStream.str() + " Bubbles Formed");
		currencyPerSecondText.setString(currencyPerSecondStream.str() + " Bubbles Per Second");

        window.clear(sf::Color::White);
		window.draw(currencyText);
		window.draw(currencyPerSecondText);
        window.draw(clickAreaShape);
        window.display();
    }
}