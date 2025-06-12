#include "BubblesFormat.h"
#include "Buffs.h"
#include "ClickingBubbles.h"
#include "GameFileState.h"
#include "OfflineBubbles.h"
#include "Upgrades.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1600, 900 }), "Bubble Incremental");
	window.setFramerateLimit(60);

    const sf::Font font("Fonts/arial.ttf");
    
    bool isButtonPressed = false;

    time_t savedTimestamp = 0;

    // All sound buffers here
    sf::SoundBuffer rubberDuckQuackBuffer;
    rubberDuckQuackBuffer.loadFromFile("Audio/rubberDuckQuack.wav");

    sf::Sound rubberDuckQuack(rubberDuckQuackBuffer);

	// Bubbles variables here
    long double bubbles = 0.0L;
    long double displayBubbles = 0.0L;
    long double allTimeBubbles = 0.0L;

    long double baseBubblesPerClick = 1.0L;
    long double baseClickMultiplier = 1.0L;
    long double allTimeBubblesPerClick = 0.0L;

    long double bubblesPerSecond = 0.0L;

    // Buffs variables here
    bool isBubbleBuffActive = false;
    bool showBubbleBuffHitbox = false;
    float bubbleBuffDuration = 20.0f;
    float bubbleBuffMultiplier = 2.0f;
    float bubbleBuffSpawnInterval = 1.0f;

    bool isGoldenBubbleBuffActive = false;
    bool showGoldenBubbleBuffHitbox = false;
    float goldenBubbleBuffDuration = 10.0f;
    float goldenBubbleBuffMultiplier = 5.0f;
    float goldenBubbleBuffSpawnInterval = 6.0f;

    bool isRubberDuckBuffActive = false;
    bool showRubberDuckBufHitbox = false;
    float rubberDuckBuffDuration = 20.0f;
    float rubberDuckBuffMultiplier = 3.5f;
    float rubberDuckBuffSpawnInterval = 3.0f;

    srand(static_cast<unsigned>(time(0)));

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
    loadFile(savedTimestamp, bubbles, allTimeBubbles, baseBubblesPerClick, bubblesPerSecond, soapCount, handWashCount, shampooCount);
    displayBubbles = bubbles;

    offlineBubbles(savedTimestamp, bubbles, allTimeBubbles, bubblesPerSecond);

	// Initialize clocks for timing
	sf::Clock secondClock;
	sf::Clock deltaClock;

    sf::Clock bubbleBuffClock;
    sf::Clock bubbleBuffSpawnIntervalClock;
    sf::Clock bubbleBuffLifetimeClock;

	sf::Clock goldenBubbleBuffClock;
	sf::Clock goldenBubbleBuffSpawnIntervalClock;
    sf::Clock goldenBubbleBuffLifetimeClock;

    sf::Clock rubberDuckBuffClock;
	sf::Clock rubberDuckBuffSpawnIntervalClock;
    sf::Clock rubberDuckBuffLifetimeClock;

	// Initialize text objects for displaying bubbles and bubbles per second
	sf::Text bubblesText(font);
    bubblesText.setPosition({ 300, 50 });
    bubblesText.setCharacterSize(24);
	bubblesText.setFillColor(sf::Color::Black);

    sf::Text bubblesPerSecondText(font);
    bubblesPerSecondText.setPosition({ 320, 80 });
    bubblesPerSecondText.setCharacterSize(14);
    bubblesPerSecondText.setFillColor(sf::Color::Black);

    // Objects for buffs
    sf::RectangleShape bubbleBuffHitbox;
    bubbleBuffHitbox.setSize(sf::Vector2f(100, 100));
    bubbleBuffHitbox.setFillColor(sf::Color::Blue);
    bubbleBuffHitbox.setPosition({ -100, -100 });

	sf::RectangleShape goldenBubbleBuffHitbox;
    goldenBubbleBuffHitbox.setSize(sf::Vector2f(100, 100));
    goldenBubbleBuffHitbox.setFillColor(sf::Color::Yellow);
	goldenBubbleBuffHitbox.setPosition({ -100, -100 });

    sf::RectangleShape rubberDuckBuffHitbox;
    rubberDuckBuffHitbox.setSize(sf::Vector2f(100, 100));
    rubberDuckBuffHitbox.setFillColor(sf::Color::Red);
	rubberDuckBuffHitbox.setPosition({ -100, -100 });

	// Objects for clicking/upgrading areas
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
                time_t currentTimestamp = time(nullptr);
                saveFile(currentTimestamp, bubbles, allTimeBubbles, baseBubblesPerClick, bubblesPerSecond, soapCount, handWashCount, shampooCount);
                window.close();
            }
        }

        // Clicking stuff
        bool isCurrentlyPressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

		// Get the mouse position relative to the window
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f mousePositionF(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));

        // Bubbles per second buff not showing on display fix
        long double realBubblesPerSecond = bubblesPerSecond;

        if (isBubbleBuffActive)
            realBubblesPerSecond *= bubbleBuffMultiplier;

        if (isGoldenBubbleBuffActive)
            realBubblesPerSecond *= goldenBubbleBuffMultiplier;

        if (isRubberDuckBuffActive)
            realBubblesPerSecond *= rubberDuckBuffMultiplier;

        // Clicking buffs
        long double clickMultiplier = baseClickMultiplier;

        if (isBubbleBuffActive)
            clickMultiplier *= bubbleBuffMultiplier;

        if (isGoldenBubbleBuffActive)
            clickMultiplier *= goldenBubbleBuffMultiplier;

        if (isRubberDuckBuffActive)
            clickMultiplier *= rubberDuckBuffMultiplier;

		// Display bubbles and bubbles per second, along with other time logic
        float deltaTime = deltaClock.restart().asSeconds();
        float smoothingFactor = 0.5f;

        stringstream displayBubblesStream;
        stringstream bubblesPerSecondStream;

        if (realBubblesPerSecond < 10.0f)
        {
            bubblesPerSecondStream << fixed << setprecision(2) << realBubblesPerSecond;
        }

        else if (realBubblesPerSecond < 100.0f)
        {
            bubblesPerSecondStream << fixed << setprecision(1) << realBubblesPerSecond;
		}

        else
        {
            bubblesPerSecondStream << fixed << setprecision(0) << realBubblesPerSecond;
		}

        // Buff logic here
        bool bubbleBuffClicked = buffHandler(
            mousePositionF,
            window,

            bubbleBuffHitbox,

            bubbleBuffClock,
            bubbleBuffSpawnIntervalClock,
            bubbleBuffLifetimeClock,

            isBubbleBuffActive,
            showBubbleBuffHitbox,

            bubbleBuffSpawnInterval,
            bubbleBuffDuration,
            180.0f, 300.0f,

            isCurrentlyPressed,
            isButtonPressed
        );

        bool goldenBubbleBuffClicked = buffHandler(
            mousePositionF,
            window,

            goldenBubbleBuffHitbox,

            goldenBubbleBuffClock,
            goldenBubbleBuffSpawnIntervalClock,
            goldenBubbleBuffLifetimeClock,

            isGoldenBubbleBuffActive,
            showGoldenBubbleBuffHitbox,

            goldenBubbleBuffSpawnInterval,
            goldenBubbleBuffDuration,
            600.0f, 900.0f,

            isCurrentlyPressed,
            isButtonPressed
		);

        bool rubberDuckBuffClicked = buffHandler(
            mousePositionF,
            window,

            rubberDuckBuffHitbox,

            rubberDuckBuffClock,
            rubberDuckBuffSpawnIntervalClock,
            rubberDuckBuffLifetimeClock,

            isRubberDuckBuffActive,
            showRubberDuckBufHitbox,

            rubberDuckBuffSpawnInterval,
            rubberDuckBuffDuration,
            300.0f, 450.0f,

            isCurrentlyPressed,
            isButtonPressed
		);

        if (rubberDuckBuffClicked)
        {
            rubberDuckQuack.play();
        }

        // Update bubbles based on time elapsed
        if (secondClock.getElapsedTime().asSeconds() >= 1.0f)
        {
            bubbles += realBubblesPerSecond;
            allTimeBubbles += realBubblesPerSecond;
            secondClock.restart();
        }

        displayBubbles += (bubbles - displayBubbles) * smoothingFactor * deltaTime;

        // Shop/Upgrade logic here
        if (isCurrentlyPressed && !isButtonPressed && upgradeArea1.contains(mousePositionF) && allTimeBubbles >= soapUnlockThreshold)
        {
            upgradeHandler(bubbles, bubblesPerSecond, soapCost, soapBaseCost, soapCount, baseSoapPerSecond, shopInflationMultiplier);
        }

        if (isCurrentlyPressed && !isButtonPressed && upgradeArea2.contains(mousePositionF) && allTimeBubbles >= handWashUnlockThreshold)
        {
            upgradeHandler(bubbles, bubblesPerSecond, handWashCost, handWashBaseCost, handWashCount, baseHandWashPerSecond, shopInflationMultiplier);
        }

        if (isCurrentlyPressed && !isButtonPressed && upgradeArea3.contains(mousePositionF) && allTimeBubbles >= shampooUnlockThreshold)
        {
            upgradeHandler(bubbles, bubblesPerSecond, shampooCost, shampooBaseCost, shampooCount, baseShampooPerSecond, shopInflationMultiplier);
        }

        // Clicking logic
        if (isCurrentlyPressed && !isButtonPressed && clickArea.contains(mousePositionF))
        {
            clickHandler(
                bubbles,
                allTimeBubbles,
                allTimeBubblesPerClick,
                baseBubblesPerClick, 
                bubblesPerSecond,
                clickMultiplier
            );
        }

        isButtonPressed = isCurrentlyPressed;

        bubblesText.setString(formatDisplayBubbles(displayBubbles) + " Bubbles Formed");
		bubblesPerSecondText.setString(bubblesPerSecondStream.str() + " Bubbles Per Second");

        window.clear(sf::Color::White);

		window.draw(bubblesText);
		window.draw(bubblesPerSecondText);

        window.draw(clickAreaShape);

        window.draw(upgradeArea1Shape);
        window.draw(upgradeArea2Shape);
        window.draw(upgradeArea3Shape);

        if (showBubbleBuffHitbox)
        {
            window.draw(bubbleBuffHitbox);
        }

        if (showGoldenBubbleBuffHitbox)
        {
            window.draw(goldenBubbleBuffHitbox);
		}

        if (showRubberDuckBufHitbox)
        {
            window.draw(rubberDuckBuffHitbox);
        }

        window.display();
    }
}