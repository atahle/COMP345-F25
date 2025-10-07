#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cctype>

#include "GameEngine.h"

using namespace std;

using GameTransition = void (GameEngine::*)();
    
void testGameStates() {
    GameEngine game;
    string command;


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

    while (true) {
        cout << "enter a command that matches a transition or 'exit'/'quit' to exit" << endl;
        cin>> command;
        
        transform(command.begin(), command.end(), command.begin(),
            [](unsigned char c) { return tolower(c); });

        if (command == "quit" || command == "exit") {
            break; 
        }

        auto it = transitions.find(command);

        if (it != transitions.end()) {
            
            (game.*(it->second))();
            cout << "--> Current State: " << stateName[static_cast<int>(game.getCurrentState())] << endl;
        } else {
            cout << "Error: Unknown command. Please try again." << endl;
        }
    }
}

