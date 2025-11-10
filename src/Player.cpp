#include "Player.h"
#include <iostream>
#include <algorithm>
#include "Orders.h"
#include "Cards.h"
using namespace std;

// Constructor: creates a new player with an empty hand, territory list, and orders list
Player::Player(const string& playerName)
{
    name = new string(playerName);
    territories = new vector<Territory*>();
    orders = new OrdersList();
    hand = new Hand();
    reinforcementPool = new int(0);
}

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

    name = new string(*other.name);
    territories = new vector<Territory*>();
    for (Territory* t : *other.territories)
        territories->push_back(t);

    hand = new Hand(*other.hand);
    orders = new OrdersList(*other.orders);
    reinforcementPool = new int(*other.reinforcementPool);

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
        t->setOwner(nullptr);
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
    for (Territory* t : *p.territories)
        out << t->getName() << ' ';
    out << " | reinforcements: " << p.getReinforcementPool()
        << " | hand: (present) | orders: " << p.getOrders().size();
    return out;
}

// Returns a list of territories the player should defend
vector<Territory*> Player::toDefend()
{
    return *territories;
}

// Returns a list of territories the player can attack
vector<Territory*> Player::toAttack()
{
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

// Creates and adds an order of the given type to the player's list
bool Player::issueOrder()
{

    vector<Territory*> defense = toDefend();
	vector<Territory*> attack = toAttack();

    if (*reinforcementPool > 0) {
		cout << *name << ", you have " << *reinforcementPool << " reinforcements available.\n Please select a territory you want to defend \n";
		printTerritories(defense);
		int territoryIdx;
		cin >> territoryIdx;
        if (territoryIdx < 1 || territoryIdx > defense.size()) {
            cout << "Invalid choice. Order issuance aborted.\n";
            return false;
		}

		cout << "Enter number of reinforcment to deploy to " << defense[territoryIdx - 1]->getName() << ": ";
		int numReinforcements;
		cin >> numReinforcements;

        if (numReinforcements <= *reinforcementPool) {
            *reinforcementPool -= numReinforcements;
            orders->add(new Deploy(defense[territoryIdx - 1], numReinforcements));
        }
        else {
            cout << "Not enough reinforcement \n";
        }
        return false;
    } else {

	cout << *name << ", select an action:\n";
	cout << "1. move troops, 2. play a card, 3. pass\n";
	int choice;
	cin >> choice;

    switch (choice)
    {
    case 1:
    {
        cout << "do you want to 1. attack or 2. fortify?\n";

        if (cin >> choice; choice == 1) {

            if (attack.empty()) {
                cout << "No territories available to attack. Order issuance aborted.\n";
                return false;
            }
            cout << "Select a territory to attack:\n";
            printTerritories(attack);
            int attackIdx;
            cin >> attackIdx;
            if (attackIdx < 1 || attackIdx > attack.size()) {
                cout << "Invalid choice. Order issuance aborted.\n";
                return false;
            }
            cout << "Select a territory to move from:\n";
            printTerritories(defense);
            int fromIdx;
            cin >> fromIdx;
            if (fromIdx < 1 || fromIdx > defense.size()) {
                cout << "Invalid choice. Order issuance aborted.\n";
                return false;
            }
            cout << "Enter number of troops to move: ";
            int numTroops;
            cin >> numTroops;
            orders->add(new Advance(defense[fromIdx - 1], attack[attackIdx - 1], numTroops));
            return false;
        }
        else if (choice == 2) {
            cout << "Select a territory to defend:\n";
            printTerritories(defense);
            int defenseToIdx;
            cin >> defenseToIdx;
            if (defenseToIdx < 1 || defenseToIdx > defense.size()) {
                cout << "Invalid choice. Order issuance aborted.\n";
                return false;
            }
            cout << "Select a territory to move from:\n";
            printTerritories(defense);
            int fromIdx;
            cin >> fromIdx;
            if (fromIdx < 1 || fromIdx > defense.size()) {
                cout << "Invalid choice. Order issuance aborted.\n";
                return false;
            }
            cout << "Enter number of troops to move: ";
            int numTroops;
            cin >> numTroops;
            orders->add(new Advance(defense[fromIdx - 1], defense[defenseToIdx - 1], numTroops));

        }


        break;
    }
    case 2:
    {
        if (hand->isEmpty()) {
            cout << "No cards in hand. Order issuance aborted.\n";
            return false;
        }
        cout << "Select a card to play:\n";
        cout << *hand;
        int cardIdx;
        cin >> cardIdx;
        Card* selectedCard = hand->removeCard(cardIdx - 1);
        if (!selectedCard) {
            cout << "Invalid choice. Order issuance aborted.\n";
            return false;
        }
        Order* order = nullptr;

        switch (selectedCard->getType())
        {
        case Card::BOMB:           order = new Bomb(); break;
        case Card::BLOCKADE:     order = new Blockade(); break;
        case Card::AIRLIFT:      order = new Airlift(); break;
        case Card::DIPLOMACY:    order = new Negotiate(); break;
        }

        orders->add(order);

        break;
    }
    case 3: {
        cout << *name << " has chosen to pass this turn.\n";
        return true;
    }
    default:
        break;
    }

    return false;

    }
}

void Player::printTerritories(vector<Territory*> terrs) {
    for (int i = 0; i < terrs.size(); i++) {
        cout << i + 1 << ". " << terrs[i]->getName() << " | ";
	}
}

// Returns a reference to the player's list of orders
const OrdersList& Player::getOrders() const
{
    return *orders;
}


// Returns the player's name
const string& Player::getName() const
{
    return *name;
}

// Adds the given number of armies to the player's reinforcement pool
void Player::addReinforcement(int armies)
{
    if (reinforcementPool && armies > 0)
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