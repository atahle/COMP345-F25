#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <iostream>
#include <vector>
#include <string>
#include "Map.h"
#include "Player.h"

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

inline const char* const stateName[] = {
	"Start",
	"MapLoaded",
	"MapValidated",
	"PlayersAdded",
	"AssignReinforcement",
	"IssueOrders",
	"ExecuteOrders",
	"Win"
};

class GameEngine {

private:
	GameState gameState = GameState::Start;
	Map* map = nullptr;
	std::vector<Player> players;
public:
	GameEngine();
	GameEngine(const GameEngine& other);
	~GameEngine();
	GameEngine& operator=(const GameEngine& other);
	friend std::ostream& operator<<(std::ostream& out, const GameEngine& ge);

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

	GameState getCurrentState() const { return gameState; }
};

#endif