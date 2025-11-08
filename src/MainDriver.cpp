#include <iostream>
#include "GameEngineDriver.h"
#include "CardsDriver.h"
#include "PlayerDriver.h"
#include "MapDriver.h"
#include "OrdersDriver.h"

int main() {

    std::cout << " Starting Warzone Game C++ Project Tests \n";
    int choice = -1;

    do {
        std::cout << "-----------------------------------------\n";
        std::cout << "  [1] - Run testLoadMaps\n";
        std::cout << "  [2] - Run testPlayers\n";
        std::cout << "  [3] - Run testOrdersLists\n";
        std::cout << "  [4] - Run testCards\n";
        std::cout << "  [5] - Run testGameStates\n";
        std::cout << "  [6] - Run testStartupPhase\n";
        std::cout << "  [7] - Run ALL tests (sequential)\n";
        std::cout << "  [0] - Quit Program\n";
        std::cout << "-----------------------------------------\n";
        std::cout << "Enter your choice (0-7): ";

        if (!(std::cin >> choice)) {
            std::cout << "\n[Error] Invalid input. Please enter a number (0-7).\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = -1; 
            continue;
        }

        switch (choice) {
        case 1:
            testLoadMaps();
            break;
        case 2:
            testPlayers();
            break;
        case 3:
            testOrdersLists();
            break;
        case 4:
            testCards();
            break;
        case 5:
            testGameStates();
            break;
        case 6: 
            testStartupPhase(); break;
        case 7:
            std::cout << "\n[Running all tests...]\n";
            testLoadMaps();
            testPlayers();
            testOrdersLists();
            testCards();
            testGameStates();
            testStartupPhase();
            break;
        case 0:
            std::cout << "\n[Program Exit] Thank you for using the menu!\n";
            break;
        default:
            std::cout << "\n[Warning] Invalid option. Please select a number between 0 and 5.\n";
            break;
        }

    } while (choice != 0);
    

    std::cout << "\nAll tests have concluded. Exiting program.\n";

    return 0;
}