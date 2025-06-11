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

#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

using namespace std;

// Function to save the game state to a file
void saveGame(float currency, float baseCurrencyPerClick, float currencyPerSecond)
{
    ofstream saveFile("save_file.txt");
    
    if (saveFile.is_open())
    {
        saveFile << currency << endl;
        saveFile << baseCurrencyPerClick << endl;
        saveFile << currencyPerSecond << endl;

        saveFile.close();
		cout << "Game saved successfully." << endl;
    }
    else
    {
        cerr << "Unable to open save file." << endl;
    }
}

// Function to load the game state from a file
void loadGame(float &currency, float &baseCurrencyPerClick, float &currencyPerSecond)
{
    ifstream saveFile("save_file.txt");
    
    if (saveFile.is_open())
    {
        saveFile >> currency;
        saveFile >> baseCurrencyPerClick;
        saveFile >> currencyPerSecond;

        saveFile.close();
        cout << "Game loaded successfully." << endl;
    }
    else
    {
        cerr << "Unable to open save file. Starting a new game." << endl;
        currency = 0;
        baseCurrencyPerClick = 1.0f;
        currencyPerSecond = 0.0f;
    }
}

void currentObjectCost(float &currentCost, float baseCost, int objectCount, float shopInflationMultiplier)
{
    currentCost = round(baseCost * pow(shopInflationMultiplier, objectCount));
}

void upgradeHandler(float &currency, float &currencyPerSecond, float &currentCost, float baseCost, int &objectCount, float addedCurrencyPerSecond, float shopInflationMultiplier)
{
    currentObjectCost(currentCost, baseCost, objectCount, shopInflationMultiplier);
    
    if (currency < 0)
    {
        cout << "Not enough currency to purchase upgrade!" << endl;   
    }
    else
    {
        currency -= currentCost;
        objectCount++;
        currencyPerSecond += addedCurrencyPerSecond;
        cout << "Upgrade purchased (" + to_string(currentCost) + ")! Current count: " << objectCount << endl;
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1600, 900 }), "Idle Game Attempt");
	window.setFramerateLimit(60);

    const sf::Font font("Fonts/arial.ttf");
    
    bool is_button_pressed = false;

	// Currency variables here
    float currency = 0.0f;
    float displayCurrency = 0.0f;
    float baseCurrencyPerClick = 1.0f;
    float currencyPerSecond = 0.0f;

    loadGame(currency, baseCurrencyPerClick, currencyPerSecond);
    displayCurrency = currency;

	// Shop/Upgrade variables here
    const float shopInflationMultiplier = 1.15f;

    int soapCount = 0;
    float baseSoapPerSecond = 0.1f;
    float soapCost = 10.0f;
	float soapBaseCost = 10.0f;

	// Initialize clocks for timing
	sf::Clock secondClock;
	sf::Clock deltaClock;

	// Initialize text objects for displaying currency and currency per second
	sf::Text currencyText(font);
    currencyText.setPosition({ 300, 50 });
    currencyText.setCharacterSize(24);
	currencyText.setFillColor(sf::Color::Black);

    sf::Text currencyPerSecondText(font);
    currencyPerSecondText.setPosition({ 320, 80 });
    currencyPerSecondText.setCharacterSize(14);
    currencyPerSecondText.setFillColor(sf::Color::Black);

	// Objects for clicking areas
    sf::FloatRect clickArea({ 300, 125 }, { 200, 150 });
	sf::FloatRect upgradeArea1({ 600, 125 }, { 200, 150 });

    sf::RectangleShape clickAreaShape;
    clickAreaShape.setSize(sf::Vector2f(200, 150));
    clickAreaShape.setOutlineColor(sf::Color::Red);
    clickAreaShape.setOutlineThickness(5);
    clickAreaShape.setPosition(sf::Vector2f({ 300, 125 }));

	sf::RectangleShape upgradeArea1Shape;
	upgradeArea1Shape.setSize(sf::Vector2f(200, 150));
	upgradeArea1Shape.setOutlineColor(sf::Color::Green);
	upgradeArea1Shape.setOutlineThickness(5);
	upgradeArea1Shape.setPosition(sf::Vector2f({ 600, 125 }));

    while (window.isOpen())
    {
        while (const optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                saveGame(currency, baseCurrencyPerClick, currencyPerSecond);
                window.close();

            }
        }

        float deltaTime = deltaClock.restart().asSeconds();
        float smoothingFactor = 5.0f;

        bool is_currently_pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f mousePositionF(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));

        stringstream displayCurrencyStream;
        stringstream currencyPerSecondStream;

        displayCurrencyStream << fixed << setprecision(0) << displayCurrency;
        currencyPerSecondStream << fixed << setprecision(2) << currencyPerSecond;

        if (secondClock.getElapsedTime().asSeconds() >= 1.0f)
        {
            currency += currencyPerSecond;
            secondClock.restart();
        }

        displayCurrency += (currency - displayCurrency) * smoothingFactor * deltaTime;

        // Shop/Upgrade logic here
        if (is_currently_pressed && !is_button_pressed && upgradeArea1.contains(mousePositionF))
        {
            upgradeHandler(currency, currencyPerSecond, soapCost, soapBaseCost, soapCount, baseSoapPerSecond, shopInflationMultiplier);
        }

        // Clicking logic
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
		window.draw(upgradeArea1Shape);
        window.display();
    }
}