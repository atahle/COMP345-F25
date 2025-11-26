#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <iostream>
#include <vector>
#include <string>
#include "Map.h"
#include "Player.h"
#include "Cards.h" // Include Cards.h
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
    Deck* deck{ nullptr }; // Part 4
    Player* neutralPlayer{ nullptr }; // Part 4

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
    void startTournament(std::vector<std::string> maps, std::vector<std::string> strategies, int numGames, int maxTurns);

    // Part 4: Helpers for Orders
    Player* getNeutralPlayer() const;
    std::vector<Player*> getOtherPlayers(Player* p) const;
    std::vector<Territory*> getAllTerritories() const;


    friend void testMainGameLoop();
    friend void testOrderExecution(); // Give driver access
};

//state enum -> String
const char* stateToString(GameState s);

#endif