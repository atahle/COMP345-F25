#include "Player.h"
#include <iostream>
#include <algorithm>
#include <limits>
#include "Orders.h"
#include "Cards.h"
#include "GameEngine.h" // Needed for issueOrder to find other players
#include "PlayerStrategies.h"
using namespace std;

// Constructor: creates a new player
Player::Player(const string& playerName)
{
    name = new string(playerName);
    territories = new vector<Territory*>();
    orders = new OrdersList();
    hand = new Hand();
    reinforcementPool = new int(0);
    conqueredTerritoryThisTurn = new bool(false); // Part 4
    diplomaticAllies = new vector<Player*>();   // Part 4
    strategy = new HumanPlayerStrategy();
}

Player::Player(const string& playerName, PlayerStrategy* initStrategy)
{
    name = new string(playerName);
    territories = new vector<Territory*>();
    orders = new OrdersList();
    hand = new Hand();
    reinforcementPool = new int(0);
    conqueredTerritoryThisTurn = new bool(false);
    diplomaticAllies = new vector<Player*>();

    // Assign the strategy provided
    this->strategy = initStrategy;
}

// Default constructor for Player strategy tests
Player::Player() : Player("Unnamed") {}

// Copy constructor: performs a deep copy of owned data
Player::Player(const Player& other)
{
    name = new string(*other.name);
    territories = new vector<Territory*>();
    for (Territory* t : *other.territories)
        territories->push_back(t);

    hand = new Hand(*other.hand);
    orders = new OrdersList(*other.orders);
    reinforcementPool = new int(*other.reinforcementPool);
    conqueredTerritoryThisTurn = new bool(*other.conqueredTerritoryThisTurn); // Part 4
    diplomaticAllies = new vector<Player*>(*other.diplomaticAllies);         // Part 4
    strategy = nullptr;
}

// Assignment operator: safely replaces this player's data with another's
Player& Player::operator=(const Player& other)
{
    if (this == &other) return *this;

    delete name;
    delete territories;
    delete orders;
    delete hand;
    delete reinforcementPool;
    delete conqueredTerritoryThisTurn; // Part 4
    delete diplomaticAllies;           // Part 4
    delete strategy;

    name = new string(*other.name);
    territories = new vector<Territory*>();
    for (Territory* t : *other.territories)
        territories->push_back(t);

    hand = new Hand(*other.hand);
    orders = new OrdersList(*other.orders);
    reinforcementPool = new int(*other.reinforcementPool);
    conqueredTerritoryThisTurn = new bool(*other.conqueredTerritoryThisTurn); // Part 4
    diplomaticAllies = new vector<Player*>(*other.diplomaticAllies);         // Part 4
    strategy = nullptr;

    return *this;
}

// Destructor: frees owned resources
Player::~Player()
{
    delete name;
    delete territories;
    delete orders;
    delete hand;
    delete reinforcementPool;
    delete conqueredTerritoryThisTurn; // Part 4
    delete diplomaticAllies;           // Part 4
    delete strategy;
}

// Adds a territory to the player if not already owned
void Player::addTerritory(Territory* t)
{
    if (t && find(territories->begin(), territories->end(), t) == territories->end())
    {
        territories->push_back(t);
        t->setOwner(this);
    }
}

// Removes a territory from the player if currently owned
void Player::removeTerritory(Territory* t)
{
    auto it = find(territories->begin(), territories->end(), t);
    if (it != territories->end())
    {
        territories->erase(it);
        // Note: We don't set owner to nullptr here,
        // because it's immediately set to the conqueror or Neutral.
    }
}

// Returns all territories owned by the player
const vector<Territory*>& Player::getTerritories() const
{
    return *territories;
}

// Output operator: prints player's name, territories, and order count
ostream& operator<<(ostream& out, const Player& p)
{
    out << "Player " << *p.name << " owns territories: ";
    if (p.territories->empty()) {
        out << "[None]";
    }
    else {
        for (Territory* t : *p.territories)
            out << t->getName() << ' ';
    }
    out << " | reinforcements: " << p.getReinforcementPool()
        << " | hand: (" << p.hand->size() << " cards) | orders: " << p.getOrders().size();
    return out;
}

// Returns a list of territories the player should defend
vector<Territory*> Player::toDefend()
{
    if (strategy) return strategy->toDefend(this);
    return *territories; // For now, defend all
}

// Returns a list of territories the player can attack
vector<Territory*> Player::toAttack()
{
    if (strategy) return strategy->toAttack(this);
    vector<Territory*> attackable;
    for (Territory* t : *territories)
    {
        for (Territory* adj : t->getAdjacentTerritories())
        {
            if (adj->getOwner() != this &&
                find(attackable.begin(), attackable.end(), adj) == attackable.end())
            {
                attackable.push_back(adj);
            }
        }
    }
    return attackable;
}

// Helper to select a territory from a list
Territory* selectTerritory(const string& prompt, const vector<Territory*>& list) {
    if (list.empty()) {
        cout << "No territories available for this action.\n";
        return nullptr;
    }
    cout << prompt << "\n";
    for (size_t i = 0; i < list.size(); ++i) {
        cout << "  " << (i + 1) << ". " << list[i]->getName() << " (" << list[i]->getArmyCount() << " armies)\n";
    }
    cout << "> ";
    int choice;
    cin >> choice;
    if (cin.fail() || choice < 1 || choice > static_cast<int>(list.size())) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "Invalid choice.\n";
        return nullptr;
    }
    return list[choice - 1];
}

// Helper to select a player from a list
Player* selectPlayer(const string& prompt, const vector<Player*>& list) {
    if (list.empty()) {
        cout << "No players available for this action.\n";
        return nullptr;
    }
    cout << prompt << "\n";
    for (size_t i = 0; i < list.size(); ++i) {
        cout << "  " << (i + 1) << ". " << list[i]->getName() << "\n";
    }
    cout << "> ";
    int choice;
    cin >> choice;
    if (cin.fail() || choice < 1 || choice > static_cast<int>(list.size())) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "Invalid choice.\n";
        return nullptr;
    }
    return list[choice - 1];
}

// Helper to get a number of armies
int selectArmies(const string& prompt, int max) {
    cout << prompt << " (max " << max << ")\n> ";
    int num;
    cin >> num;
    if (cin.fail() || num < 0) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "Invalid number. Defaulting to 0.\n";
        return 0;
    }
    if (num > max) {
        cout << "Exceeds maximum. Capping at " << max << ".\n";
        return max;
    }
    return num;
}

// Creates and adds an order to the player's list
bool Player::issueOrder(GameEngine* game)
{
    vector<Territory*> defense = toDefend();
    vector<Territory*> attack = toAttack();

    // 1. DEPLOYMENT PHASE
    if (*reinforcementPool > 0) {
        cout << "\n--- " << *name << ": DEPLOYMENT (" << *reinforcementPool << " remaining) --- \n";
        Territory* to = selectTerritory("Select territory to reinforce:", defense);
        if (!to) return false; // Failed to select, try again next loop

        int num = selectArmies("Enter number of armies to deploy:", *reinforcementPool);
        if (num <= 0) {
            cout << "Must deploy at least 1 army. Try again.\n";
            return false;
        }

        orders->add(new Deploy(this, to, num));
        *reinforcementPool -= num; // This is now temporary, execute() will pull from pool
        cout << "Deploy order issued. " << *reinforcementPool << " reinforcements remain.\n";
        return false; // Still has more to deploy
    }

    // 2. MAIN ORDER ISSUING PHASE
    cout << "\n--- " << *name << ": ORDER PHASE --- \n";
    cout << "Select an action:\n";
    cout << "  1. Advance (Attack/Move)\n";
    cout << "  2. Play a Card\n";
    cout << "  3. Pass (Finish turn)\n";
    cout << "> ";
    int choice;
    cin >> choice;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "Invalid input.\n";
        return false;
    }

    switch (choice)
    {
        // --- ADVANCE ---
    case 1:
    {
        cout << "Select action: 1. Attack 2. Move (Fortify)\n> ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input.\n";
            return false;
        }

        Territory* from = selectTerritory("Select territory to move FROM:", defense);
        if (!from) return false;

        Territory* to = nullptr;
        if (choice == 1) { // Attack
            to = selectTerritory("Select territory to ATTACK:", attack);
        }
        else { // Move
            to = selectTerritory("Select territory to move TO:", defense);
        }
        if (!to) return false;

        int num = selectArmies("Enter number of armies to advance:", from->getArmyCount());
        if (num <= 0) {
            cout << "Must advance at least 1 army.\n";
            return false;
        }

        orders->add(new Advance(this, from, to, num));
        cout << "Advance order issued.\n";
        return false;
    }

    // --- PLAY CARD ---
    case 2:
    {
        if (hand->isEmpty()) {
            cout << "No cards in hand.\n";
            return false;
        }
        cout << "Select a card to play:\n";
        cout << *hand << "\n> ";
        int cardIdx;
        cin >> cardIdx;

        Card* selectedCard = hand->getCard(cardIdx - 1); // Peek at card
        if (!selectedCard) {
            cout << "Invalid card choice.\n";
            return false;
        }

        Order* order = nullptr;
        switch (selectedCard->getType())
        {
        case Card::BOMB: { //
            cout << "BOMB: Select an enemy territory to bomb.\n";
            Territory* target = selectTerritory("Select target:", game->getAllTerritories()); // Show all
            if (!target) return false;
            order = new Bomb(this, target);
            break;
        }
        case Card::BLOCKADE: { //
            cout << "BLOCKADE: Select one of your territories to blockade.\n";
            Territory* target = selectTerritory("Select target:", defense);
            if (!target) return false;
            order = new Blockade(this, target);
            break;
        }
        case Card::AIRLIFT: { //
            cout << "AIRLIFT: Move armies between two of your territories.\n";
            Territory* from = selectTerritory("Select territory to airlift FROM:", defense);
            if (!from) return false;
            Territory* to = selectTerritory("Select territory to airlift TO:", defense);
            if (!to) return false;
            int num = selectArmies("Enter number of armies to airlift:", from->getArmyCount());
            order = new Airlift(this, from, to, num);
            break;
        }
        case Card::DIPLOMACY: { //
            cout << "DIPLOMACY: Select an enemy player to negotiate with.\n";
            Player* target = selectPlayer("Select target player:", game->getOtherPlayers(this));
            if (!target) return false;
            order = new Negotiate(this, target);
            break;
        }
        case Card::REINFORCEMENT: {
            cout << "ERROR: Reinforcement card should not exist. (This is a deck-only card).\n";
            return false;
        }
        } // end switch(card)

        if (order) {
            orders->add(order);
            hand->removeCard(cardIdx - 1); // Now remove card
            cout << "Card played and order issued.\n";
        }
        return false;
    }

    // --- PASS ---
    case 3: {
        cout << *name << " has finished issuing orders.\n";
        return true; // Player is done for this turn
    }
    default:
        cout << "Invalid choice.\n";
        return false;
    }
}


void Player::printTerritories(vector<Territory*> terrs) {
    for (size_t i = 0; i < terrs.size(); i++) {
        cout << i + 1 << ". " << terrs[i]->getName() << " | ";
    }
    cout << "\n";
}

// Returns a reference to the player's list of orders
OrdersList* Player::getOrders() { return orders; }
const OrdersList& Player::getOrders() const { return *orders; }


// Returns the player's name
const string& Player::getName() const
{
    return *name;
}

// Adds/Removes armies from the player's reinforcement pool
void Player::addReinforcement(int armies)
{
    if (reinforcementPool)
        *reinforcementPool += armies;
}

// Returns the number of armies in the player's reinforcement pool
int Player::getReinforcementPool() const
{
    return reinforcementPool ? *reinforcementPool : 0;
}

// Adds the given card to the player's hand
void Player::addCardToHand(Card* card)
{
    if (hand && card)
        hand->addCard(card);
}

Hand* Player::getHand() { return hand; }


// --- Part 4 Methods ---

void Player::setConqueredTerritory(bool value) {
    *conqueredTerritoryThisTurn = value;
}

bool Player::getConqueredTerritory() const {
    return *conqueredTerritoryThisTurn;
}

void Player::addDiplomaticAlly(Player* otherPlayer) {
    if (find(diplomaticAllies->begin(), diplomaticAllies->end(), otherPlayer) == diplomaticAllies->end()) {
        diplomaticAllies->push_back(otherPlayer);
    }
}

bool Player::isDiplomaticAlly(Player* otherPlayer) const {
    return find(diplomaticAllies->begin(), diplomaticAllies->end(), otherPlayer) != diplomaticAllies->end();
}

void Player::clearTurnEffects() {
    delete conqueredTerritoryThisTurn; // Delete old bool
    conqueredTerritoryThisTurn = new bool(false); // Create new one
    diplomaticAllies->clear();
}

void Player::setStrategy(PlayerStrategy* newStrategy) {
    if (this->strategy != nullptr) {
        delete this->strategy;
    }
    this->strategy = newStrategy;
}

void Player::issueOrder() {
    if (strategy) strategy->issueOrder(this);
}
