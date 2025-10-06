#include <iostream>
// #include "Map.h"
// #include "Player.h"
using namespace std;

enum class GameState{
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
	GameState gameState = GameState::Start;
	
	void loadMap() { //return Map
		//MapLoader ml;

		cout << "Loading Map..." << endl;
		gameState = GameState::MapLoaded;
	};

	void validateMap() { //return Map
		cout << "Validating Map..." << endl;
		gameState = GameState::MapValidated;
	};

	void addPlayer() {
		cout << "Adding players..." << endl;
		cout << "how many players do you want to add" << endl;
		cin >> numberOfPlayers;
		//player added
		gameState = GameState::PlayersAdded;
	}

	void issueOrder() { //return Map
		cout << "Validating Map..." << endl;
		gameState = GameState::MapValidated;
	};

	void endIssueOrders() { //return Map
		cout << "Validating Map..." << endl;
		gameState = GameState::MapValidated;
	};

	void execOrder() { //return Map
		cout << "Validating Map..." << endl;
		gameState = GameState::MapValidated;
	};

	void endExecOrder() { //return Map
		cout << "Validating Map..." << endl;
		gameState = GameState::MapValidated;
	};

	void win() { //return Map
		cout << "Validating Map..." << endl;
		gameState = GameState::MapValidated;
	};

public:
	int numberOfPlayers;
	int map;
	//Map map{};
	//vector<Player> players(numberOfPlayers);

	void startGame() {

		//Start Game
		loadMap();

		validateMap();

		addPlayer();

		while(gameState != GameState::Win)
		{
			switch (gameState) {
			case GameState::AssignReinforcement: { break; }
			case GameState::IssueOrders: { break; }
			case GameState::ExecuteOrders: { break; }

					
			}
		}


	}
};