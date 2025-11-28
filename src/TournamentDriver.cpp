#include "CommandProcessing.h"
#include <iostream>
#include <limits>


void testTournament() {
    std::cout << "\n=== testCommandProcessor (A2 Part 1) ===\n";
    GameEngine ge;
    ge.play(); // Start

   

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Choose mode:\n";
    std::cout << "  - Type '-console' for console input\n";
    std::cout << "  - Or type '-file <path>' to execute a file of commands\n> ";


    std::string modeLine;
    std::getline(std::cin, modeLine);

    while (true) {
        if (modeLine.rfind("-file", 0) == 0) {
            std::string filename = modeLine.substr(5);
            // trim leading spaces
            size_t p = filename.find_first_not_of(" \t");
            if (p != std::string::npos) filename = filename.substr(p);

            FileCommandProcessorAdapter fcp(filename);
            std::cout << "[File mode] Reading from: " << filename << "\n";




            fcp.attachEngine(&ge);
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            while (true) {
                Command* c = fcp.getCommand();
                if (!c) break;
                std::cout << *c << "\n";
                if (c->getRaw() == "quit") break;

            }
        }
        else if (modeLine.rfind("-console", 0) == 0) {
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
        } else if (modeLine == "-exit") {
            break;
        } else {
            std::cout << "Invalid mode. Please enter '-console', '-file <path>', or '-exit' to quit.\n";
		}

        std::getline(std::cin, modeLine);
	}
   


    
    

}


