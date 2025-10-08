#pragma once
#include <vector>
#include <string>
#include "Map.h"
using namespace std;

class Order;
class Hand;
class OrdersList;   // forward declaration for orders

// Represents a single player in the game
class Player
{
public:
    // Constructors / Destructor
    Player(const string& name);             // create a player with a name
    Player(const Player& other);            // copy constructor
    Player& operator=(const Player& other); // assignment operator
    ~Player();                              // destructor

    // Territory management
    void addTerritory(Territory* t);                 // adds a territory to player’s control
    void removeTerritory(Territory* t);              // removes a territory from player’s control
    const vector<Territory*>& getTerritories() const;// returns list of owned territories

    // Output stream
    friend ostream& operator<<(ostream& out, const Player& p); // prints basic player info

    // Player actions
    vector<Territory*> toDefend();                   // returns list of territories to defend
    vector<Territory*> toAttack();                   // returns list of territories to attack
    void issueOrder(const std::string& type);        // creates and adds an order of the given type

    // Accessor
    const OrdersList& getOrders() const;             // returns the player’s list of orders

private:
    string* name;                        // player's name
    vector<Territory*>* territories;     // list of territories owned by player
    OrdersList* orders;                  // list of player’s orders
    Hand* hand;                          // player’s hand of cards
};
