#pragma once
#include <vector>
#include <string>
#include "Map.h"
using namespace std;

class Order;
class Hand;
class Card;
class OrdersList;   // forward declaration for orders
class GameEngine;   // forward declaration for issueOrder

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
    bool issueOrder(GameEngine* game);               // Creates and adds an order (now requires GameEngine)
    void printTerritories(vector<Territory*> terrs); // print a list of territories

    // Accessor
    OrdersList* getOrders();             // (Changed to non-const for GameEngine::executeOrdersPhase)
    const OrdersList& getOrders() const; // (Overloaded for const contexts)


    // Reinforcement
    const std::string& getName() const;             // returns player name
    void addReinforcement(int armies);              // increases/decreases reinforcement pool by given number
    int getReinforcementPool() const;               // returns current reinforcement pool value

    // Cards
    void addCardToHand(Card* card);                 // adds given card to player's hand
    Hand* getHand();                                // (Added getter for issueOrder)

    // Part 4: Turn-based effects
    void setConqueredTerritory(bool value);         //
    bool getConqueredTerritory() const;
    void addDiplomaticAlly(Player* otherPlayer);    //
    bool isDiplomaticAlly(Player* otherPlayer) const;
    void clearTurnEffects();                        // Clears allies and conquest flag

private:
    string* name;                        // player's name
    vector<Territory*>* territories;     // list of territories owned by player
    OrdersList* orders;                  // list of player’s orders
    Hand* hand;                          // player’s hand of cards
    int* reinforcementPool;              // number of reinforcement armies available to player

    // Part 4 Data Members
    bool* conqueredTerritoryThisTurn;
    vector<Player*>* diplomaticAllies;

private:
    PlayerStrategy* strategy; // Pointer

public:
    // Constructor
    Player();
    ~Player(); // Destructor
    Player(const Player& other);
    Player& operator=(const Player& other);

    // Stream insertion
    friend std::ostream& operator<<(std::ostream& os, const Player& player);

    // Strategy methods
    void setStrategy(PlayerStrategy* newStrategy);
    void issueOrder();
    std::vector<Territory*> toAttack();
    std::vector<Territory*> toDefend();

};
