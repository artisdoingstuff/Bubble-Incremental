#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1600, 900 }), "Idle Game Attempt");
    const sf::Font font("arial.ttf");
    

    int currency = 0;
	float currencyPerSecond = 1.0f;

	sf::Clock clock;

	sf::Text currencyText(font);
    currencyText.setPosition({ 800, 50 });
    currencyText.setCharacterSize(24);
	currencyText.setString("Currency: " + std::to_string(currency));

	currencyText.setFillColor(sf::Color::Black);



    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::White);
		window.draw(currencyText);
        window.display();
    }
}