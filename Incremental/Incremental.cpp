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
#include <chrono>
#include <thread>

using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1600, 900 }), "Idle Game Attempt");
    const sf::Font font("arial.ttf");
    
    bool is_button_pressed = false;

    int currency = 0;
	float currencyPerSecond = 1.0f;

	sf::Clock clock;

	sf::Text currencyText(font);
    currencyText.setPosition({ 800, 50 });
    currencyText.setCharacterSize(24);

	currencyText.setFillColor(sf::Color::Black);

    sf::FloatRect clickArea({ 300, 250 }, { 200, 200 });

    sf::RectangleShape clickAreaShape;
    clickAreaShape.setSize(sf::Vector2f(200, 200));
    clickAreaShape.setOutlineColor(sf::Color::Red);
	clickAreaShape.setOutlineThickness(5);
    clickAreaShape.setPosition(sf::Vector2f({ 300, 250 }));

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            sf::Vector2f mousePositionF(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));

			bool is_currently_pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

            if (is_currently_pressed && !is_button_pressed && clickArea.contains(mousePositionF))
            {
                currency += 1;
            }

            is_button_pressed = is_currently_pressed;
        }

        currencyText.setString("Currency: " + to_string(currency));

        window.clear(sf::Color::White);
		window.draw(currencyText);
        window.draw(clickAreaShape);
        window.display();
    }
}