#include "PlayerStrategies.h"
#include "Player.h"

void testPlayerStrategies() {
    std::cout << "=== Player Strategies Test ===\n";

    Player* p1 = new Player("Player 1");
    Player* p2 = new Player("Player 2");
    Player* p3 = new Player("Player 3");
    Player* p4 = new Player("Player 4");
    Player* p5 = new Player("Player 5");

    // Assign strategies
    p1->setStrategy(new HumanPlayerStrategy());
    p2->setStrategy(new AggressivePlayerStrategy());
    p3->setStrategy(new BenevolentPlayerStrategy());
    p4->setStrategy(new NeutralPlayerStrategy());
    p5->setStrategy(new CheaterPlayerStrategy());

    // Demonstrate behavior
    std::cout << "\nHuman Player:\n";
    p1->issueOrder();
    p1->toAttack();
    p1->toDefend();

    std::cout << "\nAggressive Player:\n";
    p2->issueOrder();
    auto attacks = p2->toAttack();
    auto defends = p2->toDefend();
    for (auto t : attacks) std::cout << "Aggressive attacks: " << t->getName() << "\n";

    std::cout << "\nBenevolent Player:\n";
    p3->issueOrder();
    auto bDefends = p3->toDefend();
    for (auto t : bDefends) std::cout << "Benevolent defends: " << t->getName() << "\n";

    std::cout << "\nNeutral Player:\n";
    p4->issueOrder();

    std::cout << "\nCheater Player:\n";
    p5->issueOrder();

    // Dynamic strategy switching
    std::cout << "\nSwitch Neutral to Aggressive dynamically:\n";
    p4->setStrategy(new AggressivePlayerStrategy());
    p4->issueOrder();

    // Clean up
    delete p1; delete p2; delete p3; delete p4; delete p5;
}
