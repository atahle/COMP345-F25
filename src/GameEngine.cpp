#include <iostream>
#include <vector>
#include <string>
#include "GameEngine.h"
#include "Map.h"
#include "Player.h"
#include "Cards.h"
#include <algorithm>
#include <random>
#include <cstdlib>
#include <ctime>

using namespace std;


// relate the enum variable to a string equivalent to print
const char* stateToString(GameState s) {
	switch (s) {
	case GameState::Start:               return "Start";
	case GameState::MapLoaded:           return "MapLoaded";
	case GameState::MapValidated:        return "MapValidated";
	case GameState::PlayersAdded:        return "PlayersAdded";
	case GameState::AssignReinforcement: return "AssignReinforcement";
	case GameState::IssueOrders:         return "IssueOrders";
	case GameState::ExecuteOrders:       return "ExecuteOrders";
	case GameState::Win:                 return "Win";
	}
	return "Unknown";
}

//game state getter
GameState GameEngine::getCurrentState() const { return gameState; }

//----------------------
//Constructor destructor copy assignment stream etc.for class Game Engine
//----------------------

GameEngine::GameEngine()
	: gameState(GameState::Start),
	map(nullptr),
	players(new vector<Player*>()) {
}

GameEngine::~GameEngine() {
	delete map;
	if (players) {
		for (Player* p : *players) delete p;
		delete players;
	}
}

GameEngine::GameEngine(const GameEngine& other)
	: gameState(other.gameState) {
	map = other.map ? new Map(*other.map) : nullptr;
	players = new vector<Player*>();
	if (other.players) {
		for (Player* op : *other.players) players->push_back(new Player(*op));
	}
}

GameEngine& GameEngine::operator=(const GameEngine& other) {
	if (this == &other) return *this;

	delete map;
	if (players) {
		for (Player* p : *players) delete p;
		delete players;
	}

	gameState = other.gameState;
	map = other.map ? new Map(*other.map) : nullptr;
	players = new vector<Player*>();
	if (other.players) {
		for (Player* op : *other.players) players->push_back(new Player(*op));
	}
	return *this;
}


ostream& operator<<(ostream& out, const GameEngine& ge) {
	out << stateToString(ge.getCurrentState()) << "\n";
	out << "  Map: ";
	if (ge.map) out << "Loaded (Address: " << ge.map << ")\n";
	else        out << "Not Loaded\n";

	size_t n = ge.players ? ge.players->size() : 0;
	out << "  Players (" << n << "): \n";
	if (ge.players) {
		for (const Player* p : *ge.players) out << "- " << *p << "\n";
	}
	return out;
}

//----------------------
//Game Engine Transitions
//----------------------


void GameEngine::play() {
	cout << "Initializing game... " << endl;
	gameState = GameState::Start;
}


void GameEngine::end() {
	cout << "ending game... " << endl;
}


// chosing map file and loading map
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

	cout << "Map Loaded." << endl;
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
//lets the user add players too the game
void GameEngine::addPlayer() {
	cout << "Adding players..." << endl;
	string inputStr;

	while (true) {
		if (players->size() >= 6) {
			cout << "Maximum of 6 players reached.\n";
			break;
		}
		cout << "Enter the name of player #"
			<< (players ? players->size() + 1 : 1)
			<< ". Enter 'done' if you are done\n";
		getline(cin, inputStr);

		if (inputStr == "done") {
			if (players && players->size() >= 2) break;
			cout << "you must have at least 2 players\n";
			continue;
		}
		if (!inputStr.empty()) {
			players->push_back(new Player(inputStr));
		}
	}

	cout << "Players added\n";
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


// PART 2: Game startup phase IMPLEMENTATION

void GameEngine::startupPhase() {
	cout << "\n=== STARTUP PHASE ===\n";

	// 1) loadmap <filename>
	loadMap();

	// 2) validatemap
	validateMap();

	// 3) addplayer <playername>
	addPlayer();

	if (!map || !players || players->size() < 2) {
		cout << "\nStartupPhase aborted: invalid map or not enough players.\n";
		return;
	}

	// RNG for shuffles
	static std::mt19937 rng(static_cast<unsigned int>(time(nullptr)));

	// 4a) distribution of territories
	vector<Territory*> territoryList = map->getTerritories();
	if (territoryList.empty()) {
		cout << "\nStartupPhase aborted: map has no territories.\n";
		return;
	}

	std::shuffle(territoryList.begin(), territoryList.end(), rng);

	size_t playerCount = players->size();
	for (size_t i = 0; i < territoryList.size(); ++i) {
		Player* owner = players->at(i % playerCount);
		owner->addTerritory(territoryList[i]);
	}
	cout << "Territories distributed to players.\n";

	// 4b) randomize order of play
	std::shuffle(players->begin(), players->end(), rng);

	cout << "Player order:\n";
	for (Player* p : *players) {
		cout << " - " << p->getName() << "\n";
	}

	// 4c & 4d) give 50 initial armies and 2 cards to each player
	Deck deck(50);
	for (Player* p : *players) {
		p->addReinforcement(50);
		p->addCardToHand(deck.draw());
		p->addCardToHand(deck.draw());

		cout << p->getName()
			<< ": reinforcement pool = " << p->getReinforcementPool()
			<< ", drew 2 cards.\n";
	}

	// 4e) switch to play phase
	gameState = GameState::AssignReinforcement;
	cout << "\nStartup phase complete. State = AssignReinforcement.\n\n";
}