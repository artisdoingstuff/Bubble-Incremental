#include "BubbleMayhem.h"
#include "BubblesFormat.h"
#include "Buffs.h"
#include "ClickingBubbles.h"
#include "DuckVariants.h"
#include "GameFileState.h"
#include "GoldenBubblesVariants.h"
#include "OfflineBubbles.h"
#include "ObjectUpgrades.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1600, 900 }), "Bubble Incremental");
	window.setFramerateLimit(60);

    const sf::Font font("Fonts/arial.ttf");
    
    bool isButtonPressed = false;

    time_t savedTimestamp = 0;

    // All sounds here
    sf::SoundBuffer rubberDuckQuackBuffer;
    rubberDuckQuackBuffer.loadFromFile("Audio/rubberDuckQuack.wav");
    sf::Sound rubberDuckQuack(rubberDuckQuackBuffer);

    sf::SoundBuffer bubblePoppingBuffer;
	bubblePoppingBuffer.loadFromFile("Audio/bubblePopping.wav");
	sf::Sound bubblePopping(bubblePoppingBuffer);

	// Bubbles variables here
    long double bubbles = 0.0L;
    long double displayBubbles = 0.0L;
    long double allTimeBubbles = 0.0L;

    long double baseBubblesPerClick = 1.0L;
    long double clickMultiplier = 1.0L;
    long double allTimeBubblesPerClick = 0.0L;

    long double bubblesPerSecond = 0.0L;

    long double duckCounter = 0.0L;

    // Buffs variables here
    vector<BubbleMayhem> activeBubbles;
	bool isBubbleMayhemActive = false;
	float bubbleMayhemDuration = 20.0f;
	float bubbleMayhemBuffMultiplier = 2.0f;
	float bubbleMayhemSpawnInterval = 0.2f;

    bool isBubbleBuffActive = false;
    bool showBubbleBuffHitbox = false;
    float bubbleBuffDuration = 20.0f;
    float bubbleBuffMultiplier = 2.0f;
    float bubbleBuffSpawnInterval = 180.0f;

    bool isGoldenBubbleBuffActive = false;
    bool showGoldenBubbleBuffHitbox = false;
    float goldenBubbleBuffDuration = 0.0f;
    float goldenBubbleBuffMultiplier = 5.0f;
    float goldenBubbleBuffSpawnInterval = 1.0f;

    bool isRubberDuckBuffActive = false;
    bool showRubberDuckBuffHitbox = false;
    float rubberDuckBuffDuration = 0.0f;
    float rubberDuckBuffMultiplier = 1.0f;
    float rubberDuckBuffSpawnInterval = 1.0f;

    srand(static_cast<unsigned>(time(0)));

    // Upgrade variables here
    bool hasAllTimeBubblesMilestoneUpgrade1 = false;
    long double allTimeBubblesMilestoneUpgrade1Multiplier = 1.01f;
    long double allTimeBubblesMilestoneUpgrade1Cost = 100.0f;
    long double allTimeBubblesMilestoneUpgrade1UnlockThreshold = 100.0f;

    bool hasAllTimeBubblesMilestoneUpgrade2 = false;
    long double allTimeBubblesMilestoneUpgrade2Multiplier = 1.01f;
    long double allTimeBubblesMilestoneUpgrade2Cost = 250.0f;
    long double allTimeBubblesMilestoneUpgrade2UnlockThreshold = 250.0f;

    bool hasAllTimeBubblesMilestoneUpgrade3 = false;
    long double allTimeBubblesMilestoneUpgrade3Multiplier = 1.01f;
    long double allTimeBubblesMilestoneUpgrade3Cost = 500.0f;
    long double allTimeBubblesMilestoneUpgrade3UnlockThreshold = 500.0f;

    bool hasAllTimeBubblesMilestoneUpgrade4 = false;
    long double allTimeBubblesMilestoneUpgrade4Multiplier = 1.01f;
    long double allTimeBubblesMilestoneUpgrade4Cost = 500.0f;
    long double allTimeBubblesMilestoneUpgrade4UnlockThreshold = 750.0f;

    bool hasAllTimeBubblesMilestoneUpgrade5 = false;
    long double allTimeBubblesMilestoneUpgrade5Multiplier = 1.02f;
    long double allTimeBubblesMilestoneUpgrade5Cost = 1000.0f;
    long double allTimeBubblesMilestoneUpgrade5UnlockThreshold = 750.0f;

	// Object Upgrade variables here
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
    loadFile(
        savedTimestamp,

        duckCounter,
        
        bubbles,
        allTimeBubbles,
		allTimeBubblesPerClick,
        
        baseBubblesPerClick,
        clickMultiplier,
        
        bubblesPerSecond,
        
        soapCount,
        handWashCount,
        shampooCount
    );
    displayBubbles = bubbles;

    offlineBubbles(savedTimestamp, bubbles, allTimeBubbles, bubblesPerSecond);

	// Initialize clocks for timing
	sf::Clock secondClock;
	sf::Clock deltaClock;

	sf::Clock bubbleMayhemClock;
	sf::Clock bubbleMayhemSpawnIntervalClock;

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

	// Objects for clicking
    sf::FloatRect clickArea({ 300, 350 }, { 200, 150 });

    sf::RectangleShape clickAreaShape;
    clickAreaShape.setSize(sf::Vector2f(200, 150));
    clickAreaShape.setOutlineColor(sf::Color::Red);
    clickAreaShape.setOutlineThickness(5);
    clickAreaShape.setPosition(sf::Vector2f({ 300, 350 }));

	// Objects for upgrades
	sf::FloatRect upgradeArea1({ 1200, 50 }, { 200, 50 });

	// Objects for object upgrades
    sf::FloatRect upgradeObjectArea1({ 1200, 120 }, { 200, 50 });
    sf::FloatRect upgradeObjectArea2({ 1200, 185 }, { 200, 50 });
    sf::FloatRect upgradeObjectArea3({ 1200, 250 }, { 200, 50 });

    sf::RectangleShape upgradeObjectArea1Shape;
    upgradeObjectArea1Shape.setSize(sf::Vector2f(200, 50));
    upgradeObjectArea1Shape.setOutlineColor(sf::Color::Green);
    upgradeObjectArea1Shape.setOutlineThickness(5);
    upgradeObjectArea1Shape.setPosition(sf::Vector2f({ 1200, 120 }));

    sf::RectangleShape upgradeObjectArea2Shape;
    upgradeObjectArea2Shape.setSize(sf::Vector2f(200, 50));
    upgradeObjectArea2Shape.setOutlineColor(sf::Color::Green);
    upgradeObjectArea2Shape.setOutlineThickness(5);
    upgradeObjectArea2Shape.setPosition(sf::Vector2f({ 1200, 185 }));

    sf::RectangleShape upgradeObjectArea3Shape;
    upgradeObjectArea3Shape.setSize(sf::Vector2f(200, 50));
    upgradeObjectArea3Shape.setOutlineColor(sf::Color::Green);
    upgradeObjectArea3Shape.setOutlineThickness(5);
    upgradeObjectArea3Shape.setPosition(sf::Vector2f({ 1200, 250 }));

    while (window.isOpen())
    {
        while (const optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                time_t currentTimestamp = time(nullptr);
                saveFile(
                    currentTimestamp,

                    duckCounter,

                    bubbles,
                    allTimeBubbles,
					allTimeBubblesPerClick,

                    baseBubblesPerClick,
                    clickMultiplier,

                    bubblesPerSecond,

                    soapCount,
                    handWashCount,
                    shampooCount
                );
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
        long double realBubbles = bubbles;

        if (isBubbleMayhemActive)
            realBubblesPerSecond *= bubbleMayhemBuffMultiplier;

        if (isBubbleBuffActive)
            realBubblesPerSecond *= bubbleBuffMultiplier;

        if (isGoldenBubbleBuffActive)
            realBubblesPerSecond *= goldenBubbleBuffMultiplier;

        if (isRubberDuckBuffActive)
            realBubblesPerSecond *= rubberDuckBuffMultiplier;

        // Clicking buffs
        long double realClickMultiplier = clickMultiplier;

        if (isBubbleBuffActive)
            realClickMultiplier *= bubbleBuffMultiplier;

        if (isGoldenBubbleBuffActive)
            realClickMultiplier *= goldenBubbleBuffMultiplier;

        if (isRubberDuckBuffActive)
            realClickMultiplier *= rubberDuckBuffMultiplier;

		// Display bubbles and bubbles per second, along with other time logic
        float deltaTime = deltaClock.restart().asSeconds();
        float  smoothingFactor = 0.8f;

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
            bubbleBuffMultiplier,
            bubbleBuffDuration,
            180.0f, 300.0f,

            isCurrentlyPressed,
            isButtonPressed
        );

        if (bubbleBuffClicked)
        {
            bubblePopping.play();
        }

        goldenBubbleBuffVariant currentGoldenBubbleType;

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
			goldenBubbleBuffMultiplier,
            goldenBubbleBuffDuration,
            600.0f, 900.0f,

            isCurrentlyPressed,
            isButtonPressed,

            true,

            [&](sf::RectangleShape& buffHitbox, float& buffMultiplier, float& buffDuration, buffVariantType& buffType)
            {
                auto variant = selectgoldenBubbleVariant(buffHitbox, buffMultiplier, buffDuration);
                buffType = variant.buffType;
                currentGoldenBubbleType = variant;
            },
            [&](buffVariantType type)
            {
                if (type == buffVariantType::goldenBubbleBuff && currentGoldenBubbleType.goldenBubbleType == goldenBubbleVariantType::BubbleMayhem)
                {
                    isBubbleMayhemActive = true;
					cout << "Bubble Mayhem activated!" << endl;
                    bubbleMayhemClock.restart();
                    bubbleMayhemSpawnIntervalClock.restart();
                }
            }
		);

        if (goldenBubbleBuffClicked)
        {
            bubblePopping.play();
        }

        duckBuffVariant currentDuckType;

        bool rubberDuckBuffClicked = buffHandler(
            mousePositionF,
            window,

            rubberDuckBuffHitbox,

            rubberDuckBuffClock,
            rubberDuckBuffSpawnIntervalClock,
            rubberDuckBuffLifetimeClock,

            isRubberDuckBuffActive,
            showRubberDuckBuffHitbox,

            rubberDuckBuffSpawnInterval,
			rubberDuckBuffMultiplier,
            rubberDuckBuffDuration,
            300.0f, 450.0f,

            isCurrentlyPressed,
            isButtonPressed,

            true,
            
            [&](sf::RectangleShape& buffHitbox, float& buffMultiplier, float& buffDuration, buffVariantType& buffType)
            {
                auto variant = selectDuckVariant(buffHitbox, buffMultiplier, buffDuration);
                buffType = variant.buffType;
                currentDuckType = variant;
            },
            [&](buffVariantType type)
            {
                if (type == buffVariantType::rubberDuckBuff && currentDuckType.duckType == duckVariantType::Common)
                {
					bubbles += realBubblesPerSecond * 60;
					cout << "Common duck buff activated!" << endl;
                }

                else if (type == buffVariantType::rubberDuckBuff && currentDuckType.duckType == duckVariantType::Uncommon)
                {
                    bubbles += realBubbles * 0.05f;
                }
            }
		);

        if (rubberDuckBuffClicked)
        {
            duckCounter++;
            rubberDuckQuack.play();
        }

            if (isBubbleMayhemActive)
            {
                if (bubbleMayhemSpawnIntervalClock.getElapsedTime().asSeconds() >= bubbleMayhemSpawnInterval)
                {
                    float x = static_cast<float>(rand() % (1600 - 50));
                    float y = static_cast<float>(rand() % (900 - 50));
                    activeBubbles.emplace_back(sf::Vector2f(x, y));
                    cout << "Bubble spawned at: (" << x << ", " << y << ")" << endl;
                    bubbleMayhemSpawnIntervalClock.restart();
                }

                activeBubbles.erase(
                    remove_if(activeBubbles.begin(), activeBubbles.end(),
                        [](const BubbleMayhem& bubbleMayhem) { return bubbleMayhem.isTimeExpired(); }),
                    activeBubbles.end()
                );

                if (bubbleMayhemClock.getElapsedTime().asSeconds() > bubbleMayhemDuration)
                {
                    isBubbleMayhemActive = false;
                    activeBubbles.clear();
                }
            }

        // Update bubbles based on time elapsed
        if (secondClock.getElapsedTime().asSeconds() >= 1.0f)
        {
            bubbles += realBubblesPerSecond;
            allTimeBubbles += realBubblesPerSecond;
            secondClock.restart();
        }

        if (isBubbleMayhemActive && bubbleMayhemClock.getElapsedTime().asSeconds() > bubbleMayhemDuration)
        {
            isBubbleMayhemActive = false;
        }

        displayBubbles += (bubbles - displayBubbles) * smoothingFactor * deltaTime;

        // Shop/Upgrade logic here
        if (isCurrentlyPressed && !isButtonPressed && upgradeObjectArea1.contains(mousePositionF) && allTimeBubbles >= soapUnlockThreshold)
        {
            objectUpgradeHandler(bubbles, bubblesPerSecond, soapCost, soapBaseCost, soapCount, baseSoapPerSecond, shopInflationMultiplier);
        }

        if (isCurrentlyPressed && !isButtonPressed && upgradeObjectArea2.contains(mousePositionF) && allTimeBubbles >= handWashUnlockThreshold)
        {
            objectUpgradeHandler(bubbles, bubblesPerSecond, handWashCost, handWashBaseCost, handWashCount, baseHandWashPerSecond, shopInflationMultiplier);
        }

        if (isCurrentlyPressed && !isButtonPressed && upgradeObjectArea3.contains(mousePositionF) && allTimeBubbles >= shampooUnlockThreshold)
        {
            objectUpgradeHandler(bubbles, bubblesPerSecond, shampooCost, shampooBaseCost, shampooCount, baseShampooPerSecond, shopInflationMultiplier);
        }

        // Clicking logic
        if (isCurrentlyPressed && !isButtonPressed)
        {
            if (clickArea.contains(mousePositionF))
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
            
            for (auto bubbleIterator = activeBubbles.begin(); bubbleIterator != activeBubbles.end(); )
            {
                if (bubbleIterator->hitbox.getGlobalBounds().contains(mousePositionF))
                {
                    bubbles += 10.0L * bubbleMayhemBuffMultiplier;
					cout << "Bubble popped!" + to_string(10.0L * bubbleMayhemBuffMultiplier) << endl;
                    bubbleIterator = activeBubbles.erase(bubbleIterator);
                }

                else
                {
                    ++bubbleIterator;
                }
            }
        }

        isButtonPressed = isCurrentlyPressed;

        bubblesText.setString(formatDisplayBubbles(displayBubbles) + " Bubbles Formed");
		bubblesPerSecondText.setString(formatDisplayBubbles(realBubblesPerSecond) + " Bubbles Per Second");

        window.clear(sf::Color::White);

		window.draw(bubblesText);
		window.draw(bubblesPerSecondText);

        window.draw(clickAreaShape);

        window.draw(upgradeObjectArea1Shape);
        window.draw(upgradeObjectArea2Shape);
        window.draw(upgradeObjectArea3Shape);

        for (const auto& mayhemBubbles : activeBubbles)
        {
            window.draw(mayhemBubbles.hitbox);
        }

        if (showBubbleBuffHitbox)
        {
            window.draw(bubbleBuffHitbox);
        }

        if (showGoldenBubbleBuffHitbox)
        {
            window.draw(goldenBubbleBuffHitbox);
		}

        if (showRubberDuckBuffHitbox)
        {
            window.draw(rubberDuckBuffHitbox);
        }

        window.display();
    }
}