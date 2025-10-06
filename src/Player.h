#pragma once
#include <vector>
#include <string>
#include "Map.h"
using namespace std;
class Order;
class Hand;

class Player
{
public:
    Player(const string &name);
    Player(const Player &other);
    Player &operator=(const Player &other);
    ~Player();
    void addTerritory(Territory *t);
    void removeTerritory(Territory *t);
    const vector<Territory *> &getTerritories() const;
    friend ostream &operator<<(ostream &out, const Player &p);
    vector<Territory *> toDefend();
    vector<Territory *> toAttack();
    void issueOrder(const std::string &type);

private:
    string *name;
    vector<Territory *> *territories;
    vector<Order *> *orders;
    Hand *hand;
};