#include "Player.h"
#include <iostream>
#include <algorithm>
#include "Orders.h"
#include "Cards.h"
using namespace std;

Player::Player(const string &playerName)
{
    name = new string(playerName);
    territories = new vector<Territory *>();
    orders = new vector<Order *>();
    hand = new Hand();
}

Player::Player(const Player &other)
{
    name = new string(*other.name);
    territories = new vector<Territory *>();
    for (Territory *t : *other.territories)
    {
        territories->push_back(t);
    }
    hand = new Hand(*other.hand);
    orders = new vector<Order *>();
    for (Order *o : *other.orders)
    {
        orders->push_back(o);
    }
}

Player &Player::operator=(const Player &other)
{
    if (this == &other)
        return *this; // Self-assignment check

    delete name;
    delete territories;
    delete orders;
    delete hand;

    name = new string(*other.name);
    territories = new vector<Territory *>();
    for (Territory *t : *other.territories)
    {
        territories->push_back(t);
    }
    hand = new Hand(*other.hand);
    orders = new vector<Order *>();
    for (Order *o : *other.orders)
    {
        orders->push_back(o);
    }

    return *this;
}

Player::~Player()
{
    delete name;
    delete territories;
    delete orders;
    delete hand;
}

void Player::addTerritory(Territory *t)
{
    if (t && std::find(territories->begin(), territories->end(), t) == territories->end())
    {
        territories->push_back(t);
        t->setOwner(this);
    }
}

void Player::removeTerritory(Territory *t)
{
    auto it = std::find(territories->begin(), territories->end(), t);
    if (it != territories->end())
    {
        territories->erase(it);
        t->setOwner(nullptr);
    }
}

const vector<Territory *> &Player::getTerritories() const
{
    return *territories;
}

ostream &operator<<(ostream &out, const Player &p)
{
    out << "Player " << *p.name << " owns territories: ";
    for (Territory *t : *p.territories)
    {
        out << t->getName() << " ";
    }
    return out;
}

vector<Territory *> Player::toDefend()
{
    return *territories;
}

vector<Territory *> Player::toAttack()
{
    vector<Territory *> attackable;
    for (Territory *t : *territories)
    {
        for (Territory *adj : t->getAdjacentTerritories())
        {
            if (adj->getOwner() != this &&
                std::find(attackable.begin(), attackable.end(), adj) == attackable.end())
            {
                attackable.push_back(adj);
            }
        }
    }
    return attackable;
}

void Player::issueOrder(const std::string &type)
{
    Order *o = nullptr;

    if (type == "deploy")
        o = new Deploy();
    else if (type == "advance")
        o = new Advance();
    else if (type == "bomb")
        o = new Bomb();
    else if (type == "blockade")
        o = new Blockade();
    else if (type == "airlift")
        o = new Airlift();
    else if (type == "negotiate")
        o = new Negotiate();
    else
    {
        o = new Advance();
    }

    orders->push_back(o);
}
