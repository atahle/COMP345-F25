#include "PlayerStrategies.h"
#include "Player.h"

void testPlayerStrategies() {

    std::cout << "\nRunning testPlayerStrategies:\n\n";

    Player* human = new Player();
    Player* aggressive = new Player();
    Player* benevolent = new Player();
    Player* neutral = new Player();
    Player* cheater = new Player();

    // Assign strategies
    human->setStrategy(new HumanPlayerStrategy());
    aggressive->setStrategy(new AggressivePlayerStrategy());
    benevolent->setStrategy(new BenevolentPlayerStrategy());
    neutral->setStrategy(new NeutralPlayerStrategy());
    cheater->setStrategy(new CheaterPlayerStrategy());

    // Test issuing orders
    human->issueOrder();
    aggressive->issueOrder();
    benevolent->issueOrder();
    neutral->issueOrder();
    cheater->issueOrder();

    // Dynamic strategy change for Neutral
    std::cout << "Neutral player attacked! Changing to Aggressive...\n";
    neutral->setStrategy(new AggressivePlayerStrategy());
    neutral->issueOrder();

    // Clean up
    delete human;
    delete aggressive;
    delete benevolent;
    delete neutral;
    delete cheater;

    std::cout << "\nCompleted testPlayerStrategies.\n\n";
}
