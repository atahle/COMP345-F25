#include <iostream>
#include <vector>
#include <string>
#include "GameEngine.h"
#include "Map.h"
#include "Player.h"
using namespace std;

GameEngine::~GameEngine() {
	delete map;
}

GameEngine::GameEngine() {

}

GameEngine::GameEngine(const GameEngine& other) {
	if (other.map != nullptr) {
		this->map = new Map(*other.map);
	}
	else {
		this->map = nullptr;
	}

	this->gameState = other.gameState;
	this->players = other.players;
}

GameEngine& GameEngine::operator=(const GameEngine& other) {
	if (this == &other) {
		return *this;
	}

	delete map;

	gameState = other.gameState;
	players = other.players;

	if (other.map) {
		map = new Map(*other.map);
	}
	else {
		map = nullptr;
	}

	
	return *this;
}

ostream& operator<<(std::ostream& out, const GameEngine& ge) {
	out << "  State: " << stateName[static_cast<int>(ge.getCurrentState())] << "\n";
	out << "  Map: ";
	if (ge.map) {
		out << "Loaded (Address: " << ge.map << ")\n";
	}
	else {
		out << "Not Loaded\n";
	}

	out << "  Players (" << ge.players.size() << "): \n";
	if (!ge.players.empty()) {
		for (const auto& player : ge.players) {
			out << "- " << player << "\n";
		}
	}

	return out;
}

void GameEngine::play() {
	cout << "Initializing game... " << endl;
	gameState = GameState::Start;
}


void GameEngine::end() {
	cout << "ending game... " << endl;
}

void GameEngine::loadMap() {
	string path;
	MapLoader ml;

	if (map != nullptr) {
		cout << "Removing previous map " << endl;
		delete map;
		map = nullptr;
	}

	while (true) {
		cout << "Enter the map path: ";
		getline(cin, path);
		cout << "Loading Map..." << endl;
		
		map = ml.loadMap(path);

		if (map == nullptr) {
			cout << "There was an error loading the map" << endl;
		}
		else {
			break;
		}
	}

	cout << "Map Loaded.";
	gameState = GameState::MapLoaded;
}

void GameEngine::validateMap() {
	cout << "Validating Map..." << endl;

	while (!map->validate()) {
		cout << "Error validating map, try another map file" << endl;
		loadMap();
	}
	
	gameState = GameState::MapValidated;

}

void GameEngine::addPlayer() {
	cout << "Adding players..." << endl;
	bool doneFlag = false;
	string inputStr ;

	while (!doneFlag) {
		cout << "Enter the name of player #"<< players.size() + 1 << ". Enter 'done' if you are done" << endl;
		getline(cin, inputStr);
		if (inputStr != "done" && !inputStr.empty()) {
			players.emplace_back(inputStr);
		}
		else if (players.size() < 2){
			cout << "you must have at least 2 players" << endl;
		}
		else {
			break;
		}
	}

	if (players.empty()) {

	}

	cout << "Players added";

	gameState = GameState::PlayersAdded;
}

void GameEngine::assignCountries() {
	cout << "Assigning countries..." << endl;
	//map->assign countries ( players )

	gameState = GameState::AssignReinforcement;
}

void GameEngine::issueOrder() {
	cout << "issuing order..." << endl;
	gameState = GameState::IssueOrders;
}

void GameEngine::endIssueOrders() {
	cout << "stopped issuing orders..." << endl;
	gameState = GameState::ExecuteOrders;
}

void GameEngine::execOrder() {
	cout << "executing order..." << endl;
	gameState = GameState::ExecuteOrders;
}

void GameEngine::endExecOrder() {
	cout << "stopped executing orders..." << endl;
	gameState = GameState::AssignReinforcement;
}

void GameEngine::win() {
	cout << "a player has won the game..." << endl;
	gameState = GameState::Win;


	string inputStr;
	cout << "enter 'y' if you want to play another game" << endl;
	cin >> inputStr;

	if (inputStr == "y") {
		play();
	}
	else
	{
		end();
	}

	
}