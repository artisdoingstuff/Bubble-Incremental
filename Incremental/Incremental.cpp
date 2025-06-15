#include "BubbleFrenzy.h"
#include "BubbleChaos.h"
#include "BubbleMayhem.h"
#include "BubblesFormat.h"
#include "Buffs.h"
#include "ClickingBubbles.h"
#include "DuckVariants.h"
#include "GameFileState.h"
#include "GoldenBubblesVariants.h"
#include "ObjectUpgrades.h"
#include "OfflineBubbles.h"

// Global Textures
sf::Texture bubbleTexture;
sf::Texture goldenBubbleTexture;

// Bubble combo variables
bool isBubbleComboActive = false;
float bubbleComboResetTime = 1.5f;
int currentBubbleCombo = 0;
int highestBubbleCombo = 0;
sf::Clock bubbleComboTimer;

// Global bubble functions
template <typename ActiveBubbleType>
void handleBubbleClick(
    vector<ActiveBubbleType>& bubbleList, const sf::Vector2f& mousePositionF,
    long double& bubbles, float bubbleMultiplier, long double realBubblesPerSecond,
    sf::Sound& bubblePoppingSound, int minMult = 3, int maxMult = 30)
{
    for (auto bubbleIterator = bubbleList.begin(); bubbleIterator != bubbleList.end(); )
    {
        if (bubbleIterator->hitbox.getGlobalBounds().contains(mousePositionF) && !bubbleIterator->isBubblePopping)
        {
            if (!isBubbleComboActive)
            {
                isBubbleComboActive = true;
                bubbleComboTimer.restart();
                currentBubbleCombo = 1;
            }

            else if (bubbleComboTimer.getElapsedTime().asSeconds() <= bubbleComboResetTime)
            {
                currentBubbleCombo++;
                bubbleComboTimer.restart();
            }
            else

            {
                isBubbleComboActive = true;
                currentBubbleCombo = 1;
                bubbleComboTimer.restart();
            }

            long double bubbleComboMultiplier = 1.0 + (static_cast<long double>(currentBubbleCombo - 1) * 0.1L);
            int randomMultiplier = (rand() % (maxMult - minMult)) + minMult;
            
			long double bubbleGain = realBubblesPerSecond * randomMultiplier * bubbleMultiplier * bubbleComboMultiplier;
            
            bubbles += bubbleGain;

            bubblePoppingSound.play();
            bubbleIterator->startPoppingBubbles();
        }

        ++bubbleIterator;
    }
}

template <typename SpawnBubbleEvent>
void updateBubbleBuff(
    bool& isBuffActive,
    float buffDuration,
    float spawnInterval,
    sf::Clock& buffClock,
    sf::Clock& spawnClock,
    vector<SpawnBubbleEvent>& activeBubbleList,
    int maxX = 1600,
    int maxY = 900
)
{
    if (!isBuffActive)
        return;

    if (spawnClock.getElapsedTime().asSeconds() >= spawnInterval)
    {
        float x = static_cast<float>(rand() % maxX);
        float y = static_cast<float>(rand() % maxY);
        activeBubbleList.emplace_back(sf::Vector2f(x, y));
        spawnClock.restart();
    }

    if (buffClock.getElapsedTime().asSeconds() > buffDuration)
    {
        isBuffActive = false;
    }
}

template <typename ClearBubbleEvent>
void cleanupExpiredBubbles(vector<ClearBubbleEvent>& bubbles)
{
    bubbles.erase(
        remove_if(
            bubbles.begin(), bubbles.end(),
            [](const ClearBubbleEvent& bubble)
            {
                return bubble.isTimeExpired() || bubble.isPoppingBubblesComplete();
            }
        ),
        bubbles.end()
    );
}

template <typename DrawBubbleEventBubbles>
void updateAndDrawBubbles(vector<DrawBubbleEventBubbles>& bubbleList, sf::RenderWindow& window)
{
    for (auto& bubble : bubbleList)
    {
        if (bubble.isBubblePopping)
            bubble.updatePoppingBubblesAnimation();
        else
            bubble.updateBubbleAlpha();

        window.draw(bubble.bubbleSprite);
    }
}

int main()
{
    goldenBubbleBuffVariant currentGoldenBubbleType{};
    duckBuffVariant currentDuckType{};

    sf::RenderWindow window(sf::VideoMode({ 1600, 900 }), "Bubble Incremental");
	window.setFramerateLimit(60);

    const sf::Font font("Assets/Fonts/arial.ttf");
    
    bool isButtonPressed = false;

    time_t savedTimestamp = 0;

	// All textures here
	bubbleTexture.loadFromFile("Assets/bubble.png");
    bubbleTexture.setSmooth(true);

	goldenBubbleTexture.loadFromFile("Assets/golden_bubble.png");
    goldenBubbleTexture.setSmooth(true);

    for (auto& variant : goldenBubbleVariants)
    {
        variant.goldenBubbleSprite.setTexture(goldenBubbleTexture, true);
    }

    // All sounds here
    sf::SoundBuffer rubberDuckQuackBuffer;
    rubberDuckQuackBuffer.loadFromFile("Assets/Audio/rubberDuckQuack.wav");
    sf::Sound rubberDuckQuack(rubberDuckQuackBuffer);

    sf::SoundBuffer bubblePoppingBuffer;
	bubblePoppingBuffer.loadFromFile("Assets/Audio/bubblePopping.wav");
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
    vector<BubbleChaos> activeChaosBubbles;
    bool isBubbleChaosActive = false;
    float bubbleChaosDuration = 20.0f;
    float bubbleChaosBuffMultiplier = 0.7f;
    float bubbleChaosSpawnInterval = 0.001f;

    vector<BubbleFrenzy> activeFrenzyBubbles;
    bool isBubbleFrenzyActive = false;
    float bubbleFrenzyDuration = 20.0f;
    float bubbleFrenzyBuffMultiplier = 5.0f;
    float bubbleFrenzySpawnInterval = 0.5f;

    vector<BubbleMayhem> activeMayhemBubbles;
    bool isBubbleMayhemActive = false;
    float bubbleMayhemDuration = 20.0f;
    float bubbleMayhemBuffMultiplier = 2.0f;
    float bubbleMayhemSpawnInterval = 0.125f;

    bool isBubbleBuffActive = false;
    bool showBubbleBuffHitbox = false;
    float bubbleBuffDuration = 20.0f;
    float bubbleBuffMultiplier = 2.0f;
    float bubbleBuffSpawnInterval = 180.0f;

    bool isGoldenBubbleBuffActive = false;
    bool doesGoldenBubbleBuffExist = false;
    bool showGoldenBubbleBuffHitbox = false;
    float goldenBubbleBuffDuration = 0.0f;
    float goldenBubbleBuffMultiplier = 5.0f;
    float goldenBubbleBuffSpawnInterval = 1.0f;

    bool isRubberDuckBuffActive = false;
    bool showRubberDuckBuffHitbox = false;
    float rubberDuckBuffDuration = 0.0f;
    float rubberDuckBuffMultiplier = 1.0f;
    float rubberDuckBuffSpawnInterval = 180.0f;

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

    sf::Clock bubbleChaosClock;
    sf::Clock bubbleChaosSpawnIntervalClock;

    sf::Clock bubbleFrenzyClock;
    sf::Clock bubbleFrenzySpawnIntervalClock;

    sf::Clock bubbleMayhemClock;
    sf::Clock bubbleMayhemSpawnIntervalClock;

    sf::Clock bubbleBuffClock;
    sf::Clock bubbleBuffSpawnIntervalClock;
    sf::Clock bubbleBuffLifetimeClock;

	sf::Clock goldenBubbleBuffClock;
	sf::Clock goldenBubbleBuffSpawnIntervalClock;
    sf::Clock goldenBubbleBuffLifetimeClock;
    sf::Clock goldenBubblePulseClock;

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

	sf::Text bubbleComboText(font);
	bubbleComboText.setPosition({ 800, 120 });
	bubbleComboText.setCharacterSize(24);
	bubbleComboText.setFillColor(sf::Color::Black);

    // Objects for buffs
    sf::RectangleShape bubbleBuffHitbox;
    bubbleBuffHitbox.setSize(sf::Vector2f(100, 100));
    bubbleBuffHitbox.setFillColor(sf::Color::Blue);
    bubbleBuffHitbox.setPosition({ -100, -100 });

	sf::RectangleShape goldenBubbleBuffHitbox;
    goldenBubbleBuffHitbox.setSize(sf::Vector2f(100, 100));
    goldenBubbleBuffHitbox.setFillColor(sf::Color::Magenta);
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

            [&](sf::RectangleShape& buffHitbox, float& buffMultiplier, float& buffDuration)
            {
                selectGoldenBubbleVariant(currentGoldenBubbleType, buffHitbox, buffMultiplier, buffDuration);
                currentGoldenBubbleType.goldenBubbleSprite.setTexture(goldenBubbleTexture);
                doesGoldenBubbleBuffExist = true;
            },
            [&]()
            {
                if (currentGoldenBubbleType.goldenBubbleType == goldenBubbleVariantType::BubbleChaos)
                {
                    isBubbleChaosActive = true;
                    bubbleChaosClock.restart();
                    bubbleChaosSpawnIntervalClock.restart();
                    doesGoldenBubbleBuffExist = false;
                }

                if (currentGoldenBubbleType.goldenBubbleType == goldenBubbleVariantType::BubbleFrenzy)
                {
                    isBubbleFrenzyActive = true;
                    bubbleFrenzyClock.restart();
                    bubbleFrenzySpawnIntervalClock.restart();
                    doesGoldenBubbleBuffExist = false;
                }

                if (currentGoldenBubbleType.goldenBubbleType == goldenBubbleVariantType::BubbleMayhem)
                {
                    isBubbleMayhemActive = true;
                    bubbleMayhemClock.restart();
                    bubbleMayhemSpawnIntervalClock.restart();
                    doesGoldenBubbleBuffExist = false;
                }
            },
            &currentGoldenBubbleType.goldenBubbleSprite,
            true
		);

        if (goldenBubbleBuffClicked)
        {
            bubblePopping.play();
        }

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
            
            [&](sf::RectangleShape& buffHitbox, float& buffMultiplier, float& buffDuration)
            {
                selectDuckVariant(buffHitbox, buffMultiplier, buffDuration);
            },
            [&]()
            {
                if (currentDuckType.duckType == duckVariantType::Common)
                {
					bubbles += realBubblesPerSecond * 60;
					cout << "Common duck buff activated!" << endl;
                }

                else if (currentDuckType.duckType == duckVariantType::Uncommon)
                {
                    bubbles += realBubbles * 0.005f;
                }
            }
		);

        if (rubberDuckBuffClicked)
        {
            duckCounter++;
            rubberDuckQuack.play();
        }

		// Update bubble combo logic
        if (isBubbleComboActive && bubbleComboTimer.getElapsedTime().asSeconds() > bubbleComboResetTime)
        {
            if (currentBubbleCombo > highestBubbleCombo)
                highestBubbleCombo = currentBubbleCombo;

            isBubbleComboActive = false;
            currentBubbleCombo = 0;
        }

		// Bubble chaos, frenzy, and mayhem logic
        updateBubbleBuff(isBubbleChaosActive, bubbleChaosDuration, bubbleChaosSpawnInterval, bubbleChaosClock, bubbleChaosSpawnIntervalClock, activeChaosBubbles);
        updateBubbleBuff(isBubbleFrenzyActive, bubbleFrenzyDuration, bubbleFrenzySpawnInterval, bubbleFrenzyClock, bubbleFrenzySpawnIntervalClock, activeFrenzyBubbles);
        updateBubbleBuff(isBubbleMayhemActive, bubbleMayhemDuration, bubbleMayhemSpawnInterval, bubbleMayhemClock, bubbleMayhemSpawnIntervalClock, activeMayhemBubbles);


		// Remove expired chaos, frenzy, and mayhem bubbles
        cleanupExpiredBubbles(activeChaosBubbles);
        cleanupExpiredBubbles(activeFrenzyBubbles);
        cleanupExpiredBubbles(activeMayhemBubbles);

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
            
            handleBubbleClick(activeChaosBubbles, mousePositionF, bubbles, realBubblesPerSecond, bubbleChaosBuffMultiplier, bubblePopping);
            handleBubbleClick(activeFrenzyBubbles, mousePositionF, bubbles, realBubblesPerSecond, bubbleFrenzyBuffMultiplier, bubblePopping);
            handleBubbleClick(activeMayhemBubbles, mousePositionF, bubbles, realBubblesPerSecond, bubbleMayhemBuffMultiplier, bubblePopping);
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

        updateAndDrawBubbles(activeChaosBubbles, window);
        updateAndDrawBubbles(activeFrenzyBubbles, window);
        updateAndDrawBubbles(activeMayhemBubbles, window);

        if (isBubbleComboActive && currentBubbleCombo > 1)
        {
            bubbleComboText.setString("Bubble Combo x" + to_string(currentBubbleCombo));
            window.draw(bubbleComboText);
        }

        if (showBubbleBuffHitbox)
        {
            window.draw(bubbleBuffHitbox);
        }

        if (showGoldenBubbleBuffHitbox)
        {
            if (doesGoldenBubbleBuffExist)
            {
                if (!isGoldenBubbleBuffActive)
                {
                    float t = goldenBubblePulseClock.getElapsedTime().asMilliseconds();
                    float pulse = 1.0f + 0.02f * sinf(t * 0.003f);

                    currentGoldenBubbleType.goldenBubbleSprite.setScale(sf::Vector2f(pulse * 0.8f, pulse * 0.8f));

                    sf::Vector2u textureSize = goldenBubbleTexture.getSize();
                    currentGoldenBubbleType.goldenBubbleSprite.setOrigin(sf::Vector2f(textureSize.x / 2.0f, textureSize.y / 2.0f));

                    sf::Vector2f hitboxCenter = {
                        goldenBubbleBuffHitbox.getPosition().x + goldenBubbleBuffHitbox.getSize().x / 2.0f,
                        goldenBubbleBuffHitbox.getPosition().y + goldenBubbleBuffHitbox.getSize().y / 2.0f
                    };

                    currentGoldenBubbleType.goldenBubbleSprite.setPosition(hitboxCenter);
                }

                window.draw(currentGoldenBubbleType.goldenBubbleSprite);
            }
            else
            {
                window.draw(goldenBubbleBuffHitbox);
            }
        }

        if (showRubberDuckBuffHitbox)
        {
            window.draw(rubberDuckBuffHitbox);
        }

        window.display();
    }
}