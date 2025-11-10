#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <iostream>
#include <vector>
#include <string>
#include "Map.h"
#include "Player.h"
#include "GameEngineDriver.h"


// Game States
enum class GameState {
    Start,
    MapLoaded,
    MapValidated,
    PlayersAdded,
    AssignReinforcement,
    IssueOrders,
    ExecuteOrders,
    Win
};


class GameEngine {
private:

    //Game General Variables
    GameState gameState{ GameState::Start };
    Map* map{ nullptr };
    std::vector<Player*>* players{ nullptr };  
public:

    //Game Constructor Destructor Copy and Stream
    GameEngine();
    GameEngine(const GameEngine& other);
    ~GameEngine();
    GameEngine& operator=(const GameEngine& other);
    friend std::ostream& operator<<(std::ostream& out, const GameEngine& ge);

    //Game transition methods
    void loadMap();
    void validateMap();
    void addPlayer();
    void assignCountries();
    void issueOrder();
    void endIssueOrders();
    void execOrder();
    void endExecOrder();
    void win();
    void play();
    void end();

    //state getter
    GameState getCurrentState() const;
    
    //startup phase
    void startupPhase();

	//mainGameLoop
	void mainGameLoop();
    
	void reinforcementPhase();
	void issueOrdersPhase();
	void executeOrdersPhase();

	friend void testMainGameLoop();
};

//state enum -> String
const char* stateToString(GameState s);

#endif