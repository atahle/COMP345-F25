#include "PlayerStrategies.h"
#include "Player.h"
#include "GameEngine.h"
#include "Orders.h"

void testPlayerStrategies(){
    std::cout << "=== Player Strategies Test ===\n";
    GameEngine game;

    

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

	game.addPlayer(p1);
    game.addPlayer(p2);
	game.addPlayer(p3);
	game.addPlayer(p4);
	game.addPlayer(p5);

	game.loadMap();
	game.validateMap();

	game.assignCountries();
    
	game.reinforcementPhase();


    // Demonstrate behavior
    std::cout << "\nHuman Player:\n";
    p1->issueOrder(&game);
    std::cout << "  > Decision (Order Issued): " << *p1->getOrders() << std::endl;
    p1->toAttack();
    p1->toDefend();



    std::cout << "\nAggressive Player:\n";
    p2->issueOrder(&game);
    auto attacks = p2->toAttack();
    auto defends = p2->toDefend();
    for (auto t : attacks) std::cout << "Aggressive toAttack: " << t->getName() << "\n";
    std::cout << "  > Decision (Order Issued): " << *p2->getOrders() << std::endl;


    std::cout << "\nBenevolent Player:\n";
    p3->issueOrder(&game);
    auto bDefends = p3->toDefend();
    for (auto t : bDefends) std::cout << "Benevolent toDefend: " << t->getName() << "\n";
    std::cout << "  > Decision (Order Issued): " << *p3->getOrders() << std::endl;

    std::cout << "\nNeutral Player:\n";
    p4->issueOrder(&game);
    std::cout << "  > Decision (Order Issued): " << *p4->getOrders() << std::endl;

    std::cout << "\nCheater Player:\n";
    p5->issueOrder(&game);
    std::cout << "  > Decision (Order Issued): " << *p5->getOrders() << std::endl;


    // Clean up
    delete p1; delete p2; delete p3; delete p4; delete p5;
}
