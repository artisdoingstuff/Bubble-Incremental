#include "Incremental.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1600, 900 }), "Bubble Incremental");
	window.setFramerateLimit(120);

    const sf::Font font("Fonts/arial.ttf");
    
    bool is_button_pressed = false;

	// Currency variables here
    long double currency = 0.0L;
    long double displayCurrency = 0.0L;
    long double allTimeCurrency = 0.0L;
    long double baseCurrencyPerClick = 1.0L;
    long double allTimeCurrencyPerClick = 0.0L;
    long double currencyPerSecond = 0.0L;

	// Shop/Upgrade variables here
    const long double shopInflationMultiplier = 1.15f;

    int soapCount = 0;
    long double baseSoapPerSecond = 0.1f;
    long double soapCost = 10.0f;
    long double soapBaseCost = 10.0f;
	long double soapUnlockThreshold = 10.0f;

    int handWashCount = 0;
	long double baseHandWashPerSecond = 0.5f;
	long double handWashCost = 100.0f;
	long double handWashBaseCost = 100.0f;
	long double handWashUnlockThreshold = 100.0f;

	int shampooCount = 0;
	long double baseShampooPerSecond = 1.0f;
	long double shampooCost = 500.0f;
	long double shampooBaseCost = 500.0f;
	long double shampooUnlockThreshold = 550.0f;

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

        // Clicking stuff
        bool is_currently_pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

		// Get the mouse position relative to the window
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f mousePositionF(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));

		// Display currency and currency per second, along with other time logic
        int logCurrency = floor(log10(displayCurrency));
        int moduloRemainder = logCurrency % 3;

        float mantissaValue = (displayCurrency / pow(10, logCurrency))* pow(10, moduloRemainder);

        float deltaTime = deltaClock.restart().asSeconds();
        float smoothingFactor = 5.0f;

        stringstream displayCurrencyStream;
        stringstream currencyPerSecondStream;

        displayCurrencyStream << fixed << setprecision(2) << mantissaValue;

        if (currencyPerSecond < 10.0f)
        {
            currencyPerSecondStream << fixed << setprecision(2) << currencyPerSecond;
        }

        else if (currencyPerSecond < 100.0f)
        {
            currencyPerSecondStream << fixed << setprecision(1) << currencyPerSecond;
		}

        else
        {
            currencyPerSecondStream << fixed << setprecision(0) << currencyPerSecond;
		}

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
			long double clickValue = clickHandler(currency, baseCurrencyPerClick, currencyPerSecond);

			allTimeCurrency += baseCurrencyPerClick + baseCurrencyPerClick * (currencyPerSecond * 0.05);
			allTimeCurrencyPerClick += baseCurrencyPerClick + baseCurrencyPerClick * (currencyPerSecond * 0.05);
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