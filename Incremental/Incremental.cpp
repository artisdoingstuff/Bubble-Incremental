#include "Incremental.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1600, 900 }), "Bubble Incremental");
	window.setFramerateLimit(120);

    const sf::Font font("Fonts/arial.ttf");
    
    bool is_button_pressed = false;

    time_t timestamp_saved = 0;

	// Bubbles variables here
    long double bubbles = 0.0L;
    long double displayBubbles = 0.0L;
    long double allTimeBubbles = 0.0L;
    long double baseBubblesPerClick = 1.0L;
    long double allTimeBubblesPerClick = 0.0L;
    long double bubblesPerSecond = 0.0L;

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
    loadFile(timestamp_saved, bubbles, allTimeBubbles, baseBubblesPerClick, bubblesPerSecond, soapCount, handWashCount, shampooCount);
    displayBubbles = bubbles;

    offlineBubbles(timestamp_saved, bubbles, allTimeBubbles, bubblesPerSecond);

	// Initialize clocks for timing
	sf::Clock secondClock;
	sf::Clock deltaClock;

	// Initialize text objects for displaying bubbles and bubbles per second
	sf::Text bubblesText(font);
    bubblesText.setPosition({ 300, 50 });
    bubblesText.setCharacterSize(24);
	bubblesText.setFillColor(sf::Color::Black);

    sf::Text bubblesPerSecondText(font);
    bubblesPerSecondText.setPosition({ 320, 80 });
    bubblesPerSecondText.setCharacterSize(14);
    bubblesPerSecondText.setFillColor(sf::Color::Black);

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
                time_t timestamp_current = time(nullptr);
                saveFile(timestamp_current, bubbles, allTimeBubbles, baseBubblesPerClick, bubblesPerSecond, soapCount, handWashCount, shampooCount);
                window.close();
            }
        }

        // Clicking stuff
        bool is_currently_pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

		// Get the mouse position relative to the window
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f mousePositionF(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));

		// Display bubbles and bubbles per second, along with other time logic
        float deltaTime = deltaClock.restart().asSeconds();
        float smoothingFactor = 0.5f;

        stringstream displayBubblesStream;
        stringstream bubblesPerSecondStream;

        if (bubblesPerSecond < 10.0f)
        {
            bubblesPerSecondStream << fixed << setprecision(2) << bubblesPerSecond;
        }

        else if (bubblesPerSecond < 100.0f)
        {
            bubblesPerSecondStream << fixed << setprecision(1) << bubblesPerSecond;
		}

        else
        {
            bubblesPerSecondStream << fixed << setprecision(0) << bubblesPerSecond;
		}

		// Update bubbles based on time elapsed
        if (secondClock.getElapsedTime().asSeconds() >= 1.0f)
        {
            bubbles += bubblesPerSecond;
			allTimeBubbles += bubblesPerSecond;
            secondClock.restart();
        }

        displayBubbles += (bubbles - displayBubbles) * smoothingFactor * deltaTime;

        // Shop/Upgrade logic here
        if (is_currently_pressed && !is_button_pressed && upgradeArea1.contains(mousePositionF) && allTimeBubbles >= soapUnlockThreshold)
        {
            upgradeHandler(bubbles, bubblesPerSecond, soapCost, soapBaseCost, soapCount, baseSoapPerSecond, shopInflationMultiplier);
        }

        if (is_currently_pressed && !is_button_pressed && upgradeArea2.contains(mousePositionF) && allTimeBubbles >= handWashUnlockThreshold)
        {
            upgradeHandler(bubbles, bubblesPerSecond, handWashCost, handWashBaseCost, handWashCount, baseHandWashPerSecond, shopInflationMultiplier);
        }

        if (is_currently_pressed && !is_button_pressed && upgradeArea3.contains(mousePositionF) && allTimeBubbles >= shampooUnlockThreshold)
        {
            upgradeHandler(bubbles, bubblesPerSecond, shampooCost, shampooBaseCost, shampooCount, baseShampooPerSecond, shopInflationMultiplier);
        }

        // Clicking logic
        if (is_currently_pressed && !is_button_pressed && clickArea.contains(mousePositionF))
        {
			long double clickValue = clickHandler(bubbles, baseBubblesPerClick, bubblesPerSecond);

			allTimeBubbles += baseBubblesPerClick + baseBubblesPerClick * (bubblesPerSecond * 0.05);
			allTimeBubblesPerClick += baseBubblesPerClick + baseBubblesPerClick * (bubblesPerSecond * 0.05);
        }

        is_button_pressed = is_currently_pressed;

        bubblesText.setString(formatDisplayBubbles(displayBubbles) + " Bubbles Formed");
		bubblesPerSecondText.setString(bubblesPerSecondStream.str() + " Bubbles Per Second");

        window.clear(sf::Color::White);

		window.draw(bubblesText);
		window.draw(bubblesPerSecondText);

        window.draw(clickAreaShape);

        window.draw(upgradeArea1Shape);
        window.draw(upgradeArea2Shape);
        window.draw(upgradeArea3Shape);

        window.display();
    }
}