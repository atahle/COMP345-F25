#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cctype>
#include <limits>
#include "GameEngine.h"
#include "Cards.h"

using namespace std;

using GameTransition = void (GameEngine::*)();
    
void testGameStates() {
    GameEngine game;
    string command;


    //link function for transitions to a string
    map<string, GameTransition> transitions = {
        {"play",            &GameEngine::play},
        {"loadmap",         &GameEngine::loadMap},
        {"validatemap",     &GameEngine::validateMap},
        {"addplayer",       &GameEngine::addPlayer},
        {"assigncountries", &GameEngine::assignCountries},
        {"issueorder",      &GameEngine::issueOrder},
        {"endissueorders",  &GameEngine::endIssueOrders},
        {"execorder",       &GameEngine::execOrder},
        {"endexecorder",    &GameEngine::endExecOrder},
        {"win",             &GameEngine::win},
        {"end",             &GameEngine::end}
    };


    //loop through 
    while (true) {
        cout << "enter a command that matches a transition or 'exit'/'quit' to exit" << endl;
        cin>> command;
        
        // to lowercase
        transform(command.begin(), command.end(), command.begin(),
            [](unsigned char c) { return tolower(c); });

        if (command == "quit" || command == "exit") {
            break; 
        }

        //execute the appropriate function if not send error message
        auto it = transitions.find(command);

        if (it != transitions.end()) {
            
            (game.*(it->second))();
            cout << "--> Current State: " << stateToString(game.getCurrentState()) << '\n';
        } else {
            cout << "Error: Unknown command. Please try again." << endl;
        }
    }
}


void testStartupPhase() {
    // clear newline from menu input
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    GameEngine engine;
    engine.startupPhase();

    std::cout << "\n[Engine after startupPhase]\n\n";
    std::cout << engine << std::endl;
}

void testMainGameLoop() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    GameEngine engine;
    std::cout << "select Map Canada for testing\n";
    engine.loadMap();
    engine.validateMap();

    Player* p1 = new Player("P1 Continent");
    Player* p2 = new Player("P2 TwoTerritories");
    Player* p3 = new Player("P3 FourTerritories");

    engine.players = new std::vector<Player*>({ p1, p2, p3 });

	p1->addTerritory(engine.map->getTerritoryByName("84"));
	p1->addTerritory(engine.map->getTerritoryByName("85"));

    p2->addTerritory(engine.map->getTerritoryByName("77"));
    p2->addTerritory(engine.map->getTerritoryByName("76"));

    p3->addTerritory(engine.map->getTerritoryByName("83"));
    p3->addTerritory(engine.map->getTerritoryByName("82"));
    p3->addTerritory(engine.map->getTerritoryByName("81"));
    p3->addTerritory(engine.map->getTerritoryByName("80"));

    
    Deck *d = new Deck(10);
    p3->addCardToHand(d->draw());
    p2->addCardToHand(d->draw());
    p1->addCardToHand(d->draw());



    std::cout << "---------------test 1-----------------\n" 
        << "p1 should have 5 reinforcement 2 for continent 3 for territories \n" <<
		"p2 should have 3 reinforcement for 2 territories (rounded but min 3) \n" <<
		"p3 should have 3 reinforcement for 4 territories (rounded down) \n";

    engine.reinforcementPhase();

    std::cout << "---------------test 2-----------------\n"
        << "players cannot issue other order as long as reinforcement pool is not 0. \n"
        << "deploy all troops on any territory\n";

    std::cout << "---------------test 3-----------------\n"
        << "when troops deployed the user can issue advance orders\n"
        << "try issuing a defense and an attack order\n";
    std::cout << "---------------test 4-----------------\n"
        << "when troops deployed the user can use cards\n"
        << "try using a card on player 3\n";

    engine.issueOrdersPhase();

    std::cout << "---------------test 5/6-----------------\n"
        << "removing all territories from p1 and p2 , p3 will win\n";
   
    p1->removeTerritory(engine.map->getTerritoryByName("84"));
    p1->removeTerritory(engine.map->getTerritoryByName("85"));

    p2->removeTerritory(engine.map->getTerritoryByName("77"));
    p2->removeTerritory(engine.map->getTerritoryByName("76"));

    engine.mainGameLoop();

}

