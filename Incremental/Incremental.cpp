#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>

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
    
    bool button_pressed = false;

    int currency = 0;
	float currencyPerSecond = 1.0f;

	sf::Clock clock;

	sf::Text currencyText(font);
    currencyText.setPosition({ 800, 50 });
    currencyText.setCharacterSize(24);

	currencyText.setFillColor(sf::Color::Black);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (sf::Mouse::isButtonPressed)
            {
                if (!button_pressed && sf::Mouse::isButtonPressed (sf::Mouse::Button::Left)) {
                    button_pressed = true;

					this_thread::sleep_for(chrono::milliseconds(100));

                    currency += 1;
                }

                else {
					button_pressed = false;
			    }

				cout << "Mouse clicked at: " << sf::Mouse::getPosition(window).x << ", " << sf::Mouse::getPosition(window).y << endl;
			}
        }

        currencyText.setString("Currency: " + to_string(currency));

        window.clear(sf::Color::White);
		window.draw(currencyText);
        window.display();
    }
}