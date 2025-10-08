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
}

// Assignment operator: safely replaces this player's data with another's
Player& Player::operator=(const Player& other)
{
    if (this == &other) return *this;

    delete name;
    delete territories;
    delete orders;
    delete hand;

    name = new string(*other.name);
    territories = new vector<Territory*>();
    for (Territory* t : *other.territories)
        territories->push_back(t);

    hand = new Hand(*other.hand);
    orders = new OrdersList(*other.orders);

    return *this;
}

// Destructor: frees owned resources
Player::~Player()
{
    delete name;
    delete territories;
    delete orders;
    delete hand;
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
    out << " | hand: (present) | orders: " << p.getOrders().size();
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
void Player::issueOrder(const string& type)
{
    Order* o = nullptr;

    if (type == "deploy")        o = new Deploy();
    else if (type == "advance")  o = new Advance();
    else if (type == "bomb")     o = new Bomb();
    else if (type == "blockade") o = new Blockade();
    else if (type == "airlift")  o = new Airlift();
    else if (type == "negotiate")o = new Negotiate();
    else                         o = new Advance();

    orders->add(o);
}

// Returns a reference to the player's list of orders
const OrdersList& Player::getOrders() const
{
    return *orders;
}
