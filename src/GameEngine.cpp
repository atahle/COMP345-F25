#include <iostream>
#include <vector>
#include <string>
#include "GameEngine.h"
#include "Map.h"
#include "Player.h"
#include "Cards.h"
#include "Orders.h"
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
	players(new vector<Player*>()),
	deck(new Deck(50)), // Part 4
	neutralPlayer(new Player("Neutral")) // Part 4
{
}

GameEngine::~GameEngine() {
	delete map;
	if (players) {
		for (Player* p : *players) delete p;
		delete players;
	}
	delete deck; // Part 4
	delete neutralPlayer; // Part 4
}

GameEngine::GameEngine(const GameEngine& other)
	: gameState(other.gameState) {
	map = other.map ? new Map(*other.map) : nullptr;
	players = new vector<Player*>();
	if (other.players) {
		for (Player* op : *other.players) players->push_back(new Player(*op));
	}
	deck = new Deck(*other.deck); // Part 4
	neutralPlayer = new Player(*other.neutralPlayer); // Part 4
}

GameEngine& GameEngine::operator=(const GameEngine& other) {
	if (this == &other) return *this;

	delete map;
	if (players) {
		for (Player* p : *players) delete p;
		delete players;
	}
	delete deck; // Part 4
	delete neutralPlayer; // Part 4

	gameState = other.gameState;
	map = other.map ? new Map(*other.map) : nullptr;
	players = new vector<Player*>();
	if (other.players) {
		for (Player* op : *other.players) players->push_back(new Player(*op));
	}
	deck = new Deck(*other.deck); // Part 4
	neutralPlayer = new Player(*other.neutralPlayer); // Part 4
	return *this;
}


ostream& operator<<(ostream& out, const GameEngine& ge) {
	out << "Current State: " << stateToString(ge.getCurrentState()) << "\n";
	out << "  Map: ";
	if (ge.map) out << "Loaded (Address: " << ge.map << ")\n";
	else        out << "Not Loaded\n";

	size_t n = ge.players ? ge.players->size() : 0;
	out << "  Players (" << n << "): \n";
	if (ge.players) {
		for (const Player* p : *ge.players) out << "    - " << *p << "\n";
	}
	out << "  Neutral Player: \n";
	out << "    - " << *ge.neutralPlayer << "\n";
	return out;
}

//----------------------
//Game Engine Transitions
//----------------------

// ... (loadMap, validateMap, addPlayer, assignCountries, etc. remain mostly the same) ...
// ... (Omitted for brevity, as they were not the focus of Part 4) ...

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
	// 4a) distribution of territories
	vector<Territory*> territoryList = map->getTerritories();
	if (territoryList.empty()) {
		cout << "\nStartupPhase aborted: map has no territories.\n";
		return;
	}
	static std::mt19937 rng(static_cast<unsigned int>(time(nullptr)));
	std::shuffle(territoryList.begin(), territoryList.end(), rng);
	size_t playerCount = players->size();
	for (size_t i = 0; i < territoryList.size(); ++i) {
		Player* owner = players->at(i % playerCount);
		owner->addTerritory(territoryList[i]);
	}
	cout << "Territories distributed to players.\n";
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
	loadMap();
	validateMap();
	addPlayer();
	if (!map || !players || players->size() < 2) {
		cout << "\nStartupPhase aborted: invalid map or not enough players.\n";
		return;
	}
	static std::mt19937 rng(static_cast<unsigned int>(time(nullptr)));
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
	std::shuffle(players->begin(), players->end(), rng);
	cout << "Player order:\n";
	for (Player* p : *players) {
		cout << " - " << p->getName() << "\n";
	}

	// Re-initialize deck
	delete deck;
	deck = new Deck(50);
	for (Player* p : *players) {
		p->addReinforcement(50);
		p->addCardToHand(deck->draw());
		p->addCardToHand(deck->draw());
		cout << p->getName()
			<< ": reinforcement pool = " << p->getReinforcementPool() // <-- FIX was p.
			<< ", drew 2 cards.\n";
	}
	gameState = GameState::AssignReinforcement;
	cout << "\nStartup phase complete. State = AssignReinforcement.\n\n";
}

void GameEngine::mainGameLoop() {
	while (gameState != GameState::Win) {
		for (auto it = players->begin(); it != players->end(); /* no increment here */) {
			Player* p = *it;
			if (p->getTerritories().empty()) {
				std::cout << "Player: " << p->getName() << " has been eliminated from the game." << std::endl;
				delete p;
				it = players->erase(it);
			}
			else {
				++it;
			}
		}

		if (players->size() < 2) {
			if (players->size() == 1) {
				cout << "Player: " << players->at(0)->getName() << " Won the Game";
			}
			else {
				cout << "All players eliminated. No winner.\n";
			}
			break;
		}

		reinforcementPhase();
		issueOrdersPhase();
		executeOrdersPhase();
	}
	win();
}

void GameEngine::reinforcementPhase() {
	// For each player, calculate and assign reinforcements
	for (Player* p : *players) {
		// Part 4: Reset turn-based effects
		p->clearTurnEffects();

		int territoryCount = static_cast<int>(p->getTerritories().size());
		int reinforcements = std::max(3, territoryCount / 3); //

		for (Continent* c : map->getContinents()) { //
			bool ownsAll = true;
			if (c->getTerritories().empty()) ownsAll = false;
			for (Territory* t : c->getTerritories()) {
				if (t->getOwner() != p) {
					ownsAll = false;
					break;
				}
			}
			if (ownsAll) {
				reinforcements += c->getBonus();
			}
		}

		p->addReinforcement(reinforcements);
		cout << p->getName()
			<< " receives " << reinforcements
			<< " reinforcements. Total pool: "
			<< p->getReinforcementPool() << "\n"; // <-- FIX was p.
	}
	gameState = GameState::IssueOrders;
}

void GameEngine::issueOrdersPhase() {
	vector<bool> playersDone(players->size(), false);
	while (!all_of(playersDone.begin(), playersDone.end(), [](bool v) {return v; })) {
		for (size_t i = 0; i < players->size(); i++) {
			if (playersDone[i]) continue;
			playersDone[i] = players->at(i)->issueOrder(this); // Pass GameEngine
		}
	}
	cout << "All players have issued their orders.\n";
	gameState = GameState::ExecuteOrders;
}


void GameEngine::executeOrdersPhase() {
	cout << "\n--- EXECUTING ORDERS --- \n";
	// 1. Execute all Deploy orders first
	cout << "Executing DEPLOY orders...\n";
	bool deploymentLeft = true;
	while (deploymentLeft) {
		deploymentLeft = false;
		for (Player* p : *players) {
			OrdersList* ol = p->getOrders();
			if (ol->size() > 0 && ol->isFirstDeploy()) {
				deploymentLeft = true;
				Order* o = ol->at(0); // Get (don't remove yet)
				cout << "  - " << p->getName() << " executing: " << *o << "\n";
				o->execute(this);
				cout << "    Effect: " << o->getEffect() << "\n";
				ol->remove(0); // Now remove
			}
		}
	}

	// 2. Execute all other orders in round-robin
	cout << "Executing all other orders...\n";
	bool ordersRemaining = true;
	while (ordersRemaining) {
		ordersRemaining = false;
		for (Player* p : *players) {
			OrdersList* ol = p->getOrders();
			if (ol->size() > 0) {
				ordersRemaining = true;
				Order* o = ol->at(0);
				cout << "  - " << p->getName() << " executing: " << *o << "\n";
				o->execute(this);
				cout << "    Effect: " << o->getEffect() << "\n";
				ol->remove(0);
			}
		}
	}

	// 3. Award cards for conquest
	cout << "Awarding cards for conquest...\n";
	for (Player* p : *players) {
		if (p->getConqueredTerritory()) {
			cout << "  - " << p->getName() << " conquered a territory and draws a card.\n";
			p->addCardToHand(deck->draw());
		}
	}

	cout << "All orders executed.\n";
	gameState = GameState::AssignReinforcement;
}

// --- Part 4 Helper Implementations ---
Player* GameEngine::getNeutralPlayer() const {
	return neutralPlayer;
}

std::vector<Player*> GameEngine::getOtherPlayers(Player* p) const {
	std::vector<Player*> others;
	for (Player* other : *players) {
		if (other != p) {
			others.push_back(other);
		}
	}
	return others;
}

std::vector<Territory*> GameEngine::getAllTerritories() const {
	if (map) {
		return map->getTerritories();
	}
	return std::vector<Territory*>();
}