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
void saveGame(float currency, float allTimeCurrency, float baseCurrencyPerClick, float currencyPerSecond, int soapCount, int handWashCount, int shampooCount)
{
    ofstream saveFile("save_file.txt");
    
    if (saveFile.is_open())
    {
        saveFile << currency << endl;
        saveFile << allTimeCurrency << endl;
        saveFile << baseCurrencyPerClick << endl;
        saveFile << currencyPerSecond << endl;

        saveFile << soapCount << endl;
        saveFile << handWashCount << endl;
        saveFile << shampooCount << endl;

        saveFile.close();
		cout << "Game saved successfully." << endl;
    }

    else
    {
        cerr << "Unable to open save file." << endl;
    }
}

// Function to load the game state from a file
void loadGame(float &currency, float &allTimeCurrency, float &baseCurrencyPerClick, float &currencyPerSecond, int &soapCount, int &handWashCount, int &shampooCount)
{
    ifstream saveFile("save_file.txt");
    
    if (saveFile.is_open())
    {
        saveFile >> currency;
        saveFile >> allTimeCurrency;
        saveFile >> baseCurrencyPerClick;
        saveFile >> currencyPerSecond;

        saveFile >> soapCount;
        saveFile >> handWashCount;
        saveFile >> shampooCount;

        saveFile.close();
        cout << "Game loaded successfully." << endl;
    }

    else
    {
        cerr << "Unable to open save file. Starting a new game." << endl;
        currency = 0;
        allTimeCurrency = 0;
        baseCurrencyPerClick = 1.0f;
        currencyPerSecond = 0.0f;

        soapCount = 0;
        handWashCount = 0;
        shampooCount = 0;
    }
}

// Current cost of an object (building)
void currentObjectCost(float &currentCost, float baseCost, int objectCount, float shopInflationMultiplier)
{
    currentCost = round(baseCost * pow(shopInflationMultiplier, objectCount));
}

// Upgrade logic so I do spaghettify this file
void upgradeHandler(float &currency, float &currencyPerSecond, float &currentCost, float baseCost, int &objectCount, float addedCurrencyPerSecond, float shopInflationMultiplier)
{
    currentObjectCost(currentCost, baseCost, objectCount, shopInflationMultiplier);

    currency -= currentCost;
    
    if (currency < 0)
    {
		currency += currentCost;
        cout << "Not enough currency to purchase upgrade!" << endl;   
    }

    else
    {
        objectCount++;
        currencyPerSecond += addedCurrencyPerSecond;
        cout << "Upgrade purchased (" + to_string(currentCost) + ")! Current count: " << objectCount << endl;
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1600, 900 }), "Bubble Incremental");
	window.setFramerateLimit(120);

    const sf::Font font("Fonts/arial.ttf");
    
    bool is_button_pressed = false;

	// Currency variables here
    float currency = 0.0f;
	float allTimeCurrency = 0.0f;
    float displayCurrency = 0.0f;
    float baseCurrencyPerClick = 1.0f;
    float currencyPerSecond = 0.0f;

	// Shop/Upgrade variables here
    const float shopInflationMultiplier = 1.15f;

    int soapCount = 0;
    float baseSoapPerSecond = 0.1f;
    float soapCost = 10.0f;
	float soapBaseCost = 10.0f;
	float soapUnlockThreshold = 10.0f;

    int handWashCount = 0;
	float baseHandWashPerSecond = 0.5f;
	float handWashCost = 100.0f;
	float handWashBaseCost = 100.0f;
	float handWashUnlockThreshold = 100.0f;

	int shampooCount = 0;
	float baseShampooPerSecond = 1.0f;
	float shampooCost = 500.0f;
	float shampooBaseCost = 500.0f;
	float shampooUnlockThreshold = 550.0f;

    // Loading game file (if it exists)
    loadGame(currency, allTimeCurrency, baseCurrencyPerClick, currencyPerSecond, soapCount, handWashCount, shampooCount);
    displayCurrency = currency;

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
    sf::FloatRect clickArea({ 300, 350 }, { 200, 150 });

    sf::FloatRect upgradeArea1({ 1200, 120 }, { 200, 50 });
    sf::FloatRect upgradeArea2({ 1200, 185 }, { 200, 50 });
    sf::FloatRect upgradeArea3({ 1200, 250 }, { 200, 50 });

    sf::RectangleShape clickAreaShape;
    clickAreaShape.setSize(sf::Vector2f(200, 150));
    clickAreaShape.setOutlineColor(sf::Color::Red);
    clickAreaShape.setOutlineThickness(5);
    clickAreaShape.setPosition(sf::Vector2f({ 300, 350 }));

    sf::RectangleShape upgradeArea1Shape;
    upgradeArea1Shape.setSize(sf::Vector2f(200, 50));
    upgradeArea1Shape.setOutlineColor(sf::Color::Green);
    upgradeArea1Shape.setOutlineThickness(5);
    upgradeArea1Shape.setPosition(sf::Vector2f({ 1200, 120 }));

    sf::RectangleShape upgradeArea2Shape;
    upgradeArea2Shape.setSize(sf::Vector2f(200, 50));
    upgradeArea2Shape.setOutlineColor(sf::Color::Green);
    upgradeArea2Shape.setOutlineThickness(5);
    upgradeArea2Shape.setPosition(sf::Vector2f({ 1200, 185 }));

    sf::RectangleShape upgradeArea3Shape;
    upgradeArea3Shape.setSize(sf::Vector2f(200, 50));
    upgradeArea3Shape.setOutlineColor(sf::Color::Green);
    upgradeArea3Shape.setOutlineThickness(5);
    upgradeArea3Shape.setPosition(sf::Vector2f({ 1200, 250 }));

    while (window.isOpen())
    {
        while (const optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                saveGame(currency, allTimeCurrency, baseCurrencyPerClick, currencyPerSecond, soapCount, handWashCount, shampooCount);
                window.close();

            }
        }

        bool is_currently_pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

		// Get the mouse position relative to the window
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f mousePositionF(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));

		// Display currency and currency per second, along with other time logic
        float deltaTime = deltaClock.restart().asSeconds();
        float smoothingFactor = 5.0f;

        stringstream displayCurrencyStream;
        stringstream currencyPerSecondStream;

        displayCurrencyStream << fixed << setprecision(0) << displayCurrency;
        currencyPerSecondStream << fixed << setprecision(2) << currencyPerSecond;

		// Update currency based on time elapsed
        if (secondClock.getElapsedTime().asSeconds() >= 1.0f)
        {
            currency += currencyPerSecond;
			allTimeCurrency += currencyPerSecond;
            secondClock.restart();
        }

        displayCurrency += (currency - displayCurrency) * smoothingFactor * deltaTime;

        // Shop/Upgrade logic here
        if (is_currently_pressed && !is_button_pressed && upgradeArea1.contains(mousePositionF) && allTimeCurrency >= soapUnlockThreshold)
        {
            upgradeHandler(currency, currencyPerSecond, soapCost, soapBaseCost, soapCount, baseSoapPerSecond, shopInflationMultiplier);
        }

        if (is_currently_pressed && !is_button_pressed && upgradeArea2.contains(mousePositionF) && allTimeCurrency >= handWashUnlockThreshold)
        {
            upgradeHandler(currency, currencyPerSecond, handWashCost, handWashBaseCost, handWashCount, baseHandWashPerSecond, shopInflationMultiplier);
        }

        if (is_currently_pressed && !is_button_pressed && upgradeArea3.contains(mousePositionF) && allTimeCurrency >= shampooUnlockThreshold)
        {
            upgradeHandler(currency, currencyPerSecond, shampooCost, shampooBaseCost, shampooCount, baseShampooPerSecond, shopInflationMultiplier);
        }

        // Clicking logic
        if (is_currently_pressed && !is_button_pressed && clickArea.contains(mousePositionF))
        {
            currency += baseCurrencyPerClick + baseCurrencyPerClick * (currencyPerSecond * 0.05);
			allTimeCurrency += baseCurrencyPerClick + baseCurrencyPerClick * (currencyPerSecond * 0.05);
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
        window.draw(upgradeArea2Shape);
        window.draw(upgradeArea3Shape);

        window.display();
    }
}