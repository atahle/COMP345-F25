#include "CommandProcessing.h"
#include <iostream>
#include <limits>


void testTournament() {
    std::cout << "\n=== testCommandProcessor (A2 Part 1) ===\n";
    GameEngine ge;
    ge.play(); // Start

    CommandProcessor cp;
    cp.attachEngine(&ge);


    

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "input command: tournament -M <listofmapfiles> -P <listofplayerstrategies> -G <numberofgames> -D <maxnumberofturns>\n";
    
    while (true) {
        Command* c = cp.getCommand();
        if (!c) break;
        std::cout << *c << "\n";
        if (c->getRaw() == "quit") break;

    }

}
