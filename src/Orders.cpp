#include "Orders.h"
#include "Map.h"
#include "Player.h"
#include "GameEngine.h"
#include <iomanip>
#include <sstream>
#include <random>
#include <algorithm>

// Helper: Check if two territories are adjacent
bool isAdjacent(Territory* t1, Territory* t2) {
    if (!t1 || !t2) return false;
    const auto& neighbors = t1->getAdjacentTerritories();
    return std::find(neighbors.begin(), neighbors.end(), t2) != neighbors.end();
}

// Helper: Check if target is adjacent to any territory owned by player
bool isAdjacentToOwned(Player* p, Territory* target) {
    if (!p || !target) return false;
    for (Territory* owned : p->getTerritories()) {
        if (isAdjacent(owned, target)) {
            return true;
        }
    }
    return false;
}

// Helper: Battle simulation
void simulateBattle(int att, int def, int& attSurvived, int& defSurvived) {
    static std::mt19937 rng(static_cast<unsigned int>(time(nullptr)));
    std::uniform_int_distribution<int> dist(1, 100);

    attSurvived = att;
    defSurvived = def;

    // Each attacker gets a 60% chance to kill one defender
    for (int i = 0; i < att; ++i) {
        if (dist(rng) <= 60 && defSurvived > 0) {
            defSurvived--;
        }
    }

    // Each defender gets a 70% chance to kill one attacker
    for (int i = 0; i < def; ++i) {
        if (dist(rng) <= 70 && attSurvived > 0) {
            attSurvived--;
        }
    }

    // Ensure no negative survivors
    if (attSurvived < 0) attSurvived = 0;
    if (defSurvived < 0) defSurvived = 0;
}


//----------------------
//Order constructor destructor copy assignment
//----------------------

Order::Order(const std::string& n, Player* issuer)
    : name(new std::string(n)),
    effect(new std::string("Not executed")),
    executed(new bool(false)),
    issuingPlayer(issuer) {
}

Order::Order(const Order& other)
    : name(new std::string(*other.name)),
    effect(new std::string(*other.effect)),
    executed(new bool(*other.executed)),
    issuingPlayer(other.issuingPlayer) // Shallow copy player pointer
{
}

Order& Order::operator=(const Order& other) {
    if (this != &other) {
        *name = *other.name;
        *effect = *other.effect;
        *executed = *other.executed;
        issuingPlayer = other.issuingPlayer;
    }
    return *this;
}

Order::~Order() {
    delete name;
    delete effect;
    delete executed;
}

//Orders getter
const std::string& Order::getName() const { return *name; }
const std::string& Order::getEffect() const { return *effect; }
bool Order::isExecuted() const { return *executed; }

std::ostream& operator<<(std::ostream& out, const Order& o) {
    out << o.getName();
    if (*o.executed) out << "\t| effect: " << o.getEffect();
    return out;
}
//----------------------
// Deploy
//----------------------
Deploy::Deploy(Player* issuer, Territory* t, int numReinforcement)
    : Order("Deploy", issuer), targetTerritory(t), numArmies(new int(numReinforcement)) {
}
Deploy::Deploy(const Deploy& other) : Order(other) {
    targetTerritory = other.targetTerritory;
    numArmies = new int(*other.numArmies);
}
Deploy::~Deploy() { delete numArmies; }
Deploy* Deploy::clone() const { return new Deploy(*this); }
bool Deploy::validate(GameEngine* game) const {
    if (*executed) return false;
    // If the target territory does not belong to the player, invalid.
    if (targetTerritory->getOwner() != issuingPlayer) {
        *effect = "Validation Failed: Target territory " + targetTerritory->getName() + " not owned by " + issuingPlayer->getName();
        return false;
    }
    return true;
}
void Deploy::execute(GameEngine* game) {
    if (*executed) return;
    if (!validate(game)) {
        *executed = true; // Mark as executed even if failed
        return;
    }

    // Add armies from pool to territory
    int armiesToDeploy = *numArmies;
    if (issuingPlayer->getReinforcementPool() < armiesToDeploy) {
        armiesToDeploy = issuingPlayer->getReinforcementPool(); // Take all remaining
    }

    issuingPlayer->addReinforcement(-armiesToDeploy); // Subtract from pool
    targetTerritory->setArmyCount(targetTerritory->getArmyCount() + armiesToDeploy);

    std::stringstream ss;
    ss << "Deployed " << armiesToDeploy << " armies to " << targetTerritory->getName();
    *effect = ss.str();
    *executed = true;
}

//----------------------
// Advance
//----------------------
Advance::Advance(Player* issuer, Territory* from, Territory* to, int numReinforcement)
    : Order("Advance", issuer), sourceTerritory(from), targetTerritory(to), numArmies(new int(numReinforcement)) {
}
Advance::Advance(const Advance& other) : Order(other) {
    sourceTerritory = other.sourceTerritory;
    targetTerritory = other.targetTerritory;
    numArmies = new int(*other.numArmies);
}
Advance::~Advance() { delete numArmies; }
Advance* Advance::clone() const { return new Advance(*this); }
bool Advance::validate(GameEngine* game) const {
    if (*executed) return false;
    // If source does not belong to player, invalid.
    if (sourceTerritory->getOwner() != issuingPlayer) {
        *effect = "Validation Failed: Source territory " + sourceTerritory->getName() + " not owned by " + issuingPlayer->getName();
        return false;
    }
    // If target is not adjacent, invalid.
    if (!isAdjacent(sourceTerritory, targetTerritory)) {
        *effect = "Validation Failed: Target " + targetTerritory->getName() + " is not adjacent to " + sourceTerritory->getName();
        return false;
    }
    // Check for diplomacy if it's an attack
    if (targetTerritory->getOwner() != issuingPlayer) {
        if (issuingPlayer->isDiplomaticAlly(targetTerritory->getOwner())) {
            *effect = "Validation Failed: Attack against " + targetTerritory->getOwner()->getName() + " cancelled due to diplomacy.";
            return false;
        }
    }
    return true;
}
void Advance::execute(GameEngine* game) {
    if (*executed) return;
    if (!validate(game)) {
        *executed = true;
        return;
    }

    int armiesToMove = *numArmies;
    if (sourceTerritory->getArmyCount() < armiesToMove) {
        armiesToMove = sourceTerritory->getArmyCount(); // Move all available
    }

    // Case 1: Friendly move
    if (targetTerritory->getOwner() == issuingPlayer) {
        sourceTerritory->setArmyCount(sourceTerritory->getArmyCount() - armiesToMove);
        targetTerritory->setArmyCount(targetTerritory->getArmyCount() + armiesToMove);

        std::stringstream ss;
        ss << "Moved " << armiesToMove << " armies from " << sourceTerritory->getName() << " to " << targetTerritory->getName();
        *effect = ss.str();
    }
    // Case 2: Attack
    else {
        Player* defender = targetTerritory->getOwner();
        int attackers = armiesToMove;
        int defenders = targetTerritory->getArmyCount();

        int attSurvived, defSurvived;
        simulateBattle(attackers, defenders, attSurvived, defSurvived);

        sourceTerritory->setArmyCount(sourceTerritory->getArmyCount() - armiesToMove); // Armies leave source regardless

        // Check for conquest
        if (defSurvived <= 0) {
            defender->removeTerritory(targetTerritory);
            issuingPlayer->addTerritory(targetTerritory); // This also sets owner
            targetTerritory->setArmyCount(attSurvived); //
            issuingPlayer->setConqueredTerritory(true); //

            std::stringstream ss;
            ss << "Attacked " << targetTerritory->getName() << " and CONQUERED it. "
                << attSurvived << " attackers survived. "
                << defender->getName() << " lost the territory.";
            *effect = ss.str();
        }
        // Attack failed, but defenders may have died
        else {
            targetTerritory->setArmyCount(defSurvived);
            std::stringstream ss;
            ss << "Attacked " << targetTerritory->getName() << " but FAILED to conquer. "
                << attSurvived << " attackers and " << defSurvived << " defenders survived.";
            *effect = ss.str();
        }
    }
    *executed = true;
}

//----------------------
// Bomb
//----------------------
Bomb::Bomb(Player* issuer, Territory* t) : Order("Bomb", issuer), targetTerritory(t) {}
Bomb::Bomb(const Bomb& other) : Order(other), targetTerritory(other.targetTerritory) {}
Bomb::~Bomb() {}
Bomb* Bomb::clone() const { return new Bomb(*this); }
bool Bomb::validate(GameEngine* game) const {
    if (*executed) return false;
    // If target belongs to player, invalid.
    if (targetTerritory->getOwner() == issuingPlayer) {
        *effect = "Validation Failed: Cannot bomb your own territory " + targetTerritory->getName();
        return false;
    }
    // If target not adjacent to one of player's territories, invalid.
    if (!isAdjacentToOwned(issuingPlayer, targetTerritory)) {
        *effect = "Validation Failed: Target " + targetTerritory->getName() + " is not adjacent to any of your territories.";
        return false;
    }
    return true;
}
void Bomb::execute(GameEngine* game) {
    if (*executed) return;
    if (!validate(game)) {
        *executed = true;
        return;
    }
    // Remove half of the army units
    int armiesRemoved = targetTerritory->getArmyCount() / 2;
    targetTerritory->setArmyCount(targetTerritory->getArmyCount() - armiesRemoved);

    std::stringstream ss;
    ss << "Bombed " << targetTerritory->getName() << ", removing " << armiesRemoved << " armies.";
    *effect = ss.str();
    *executed = true;
}

//----------------------
// Blockade
//----------------------
Blockade::Blockade(Player* issuer, Territory* t) : Order("Blockade", issuer), targetTerritory(t) {}
Blockade::Blockade(const Blockade& other) : Order(other), targetTerritory(other.targetTerritory) {}
Blockade::~Blockade() {}
Blockade* Blockade::clone() const { return new Blockade(*this); }
bool Blockade::validate(GameEngine* game) const {
    if (*executed) return false;
    // If target territory belongs to an enemy, invalid.
    if (targetTerritory->getOwner() != issuingPlayer) {
        *effect = "Validation Failed: Cannot blockade territory " + targetTerritory->getName() + " as you do not own it.";
        return false;
    }
    return true;
}
void Blockade::execute(GameEngine* game) {
    if (*executed) return;
    if (!validate(game)) {
        *executed = true;
        return;
    }
    // Double armies and transfer to Neutral player
    targetTerritory->setArmyCount(targetTerritory->getArmyCount() * 2);
    Player* neutral = game->getNeutralPlayer();

    issuingPlayer->removeTerritory(targetTerritory);
    neutral->addTerritory(targetTerritory); // This also sets owner

    std::stringstream ss;
    ss << "Blockaded " << targetTerritory->getName() << ". Army count doubled to "
        << targetTerritory->getArmyCount() << " and ownership transferred to Neutral.";
    *effect = ss.str();
    *executed = true;
}

//----------------------
// Airlift
//----------------------
Airlift::Airlift(Player* issuer, Territory* from, Territory* to, int numArmies)
    : Order("Airlift", issuer), sourceTerritory(from), targetTerritory(to), numArmies(new int(numArmies)) {
}
Airlift::Airlift(const Airlift& other) : Order(other) {
    sourceTerritory = other.sourceTerritory;
    targetTerritory = other.targetTerritory;
    numArmies = new int(*other.numArmies);
}
Airlift::~Airlift() { delete numArmies; }
Airlift* Airlift::clone() const { return new Airlift(*this); }
bool Airlift::validate(GameEngine* game) const {
    if (*executed) return false;
    // If source or target do not belong to player, invalid.
    if (sourceTerritory->getOwner() != issuingPlayer || targetTerritory->getOwner() != issuingPlayer) {
        *effect = "Validation Failed: Both source and target territories must be owned by you.";
        return false;
    }
    return true;
}
void Airlift::execute(GameEngine* game) {
    if (*executed) return;
    if (!validate(game)) {
        *executed = true;
        return;
    }

    int armiesToMove = *numArmies;
    if (sourceTerritory->getArmyCount() < armiesToMove) {
        armiesToMove = sourceTerritory->getArmyCount(); // Move all available
    }

    // Move armies from source to target
    sourceTerritory->setArmyCount(sourceTerritory->getArmyCount() - armiesToMove);
    targetTerritory->setArmyCount(targetTerritory->getArmyCount() + armiesToMove);

    std::stringstream ss;
    ss << "Airlifted " << armiesToMove << " armies from " << sourceTerritory->getName() << " to " << targetTerritory->getName();
    *effect = ss.str();
    *executed = true;
}

//----------------------
// Negotiate
//----------------------
Negotiate::Negotiate(Player* issuer, Player* target)
    : Order("Negotiate", issuer), targetPlayer(target) {
}
Negotiate::Negotiate(const Negotiate& other) : Order(other), targetPlayer(other.targetPlayer) {}
Negotiate::~Negotiate() {}
Negotiate* Negotiate::clone() const { return new Negotiate(*this); }
bool Negotiate::validate(GameEngine* game) const {
    if (*executed) return false;
    // If target is the player issuing, invalid.
    if (targetPlayer == issuingPlayer) {
        *effect = "Validation Failed: Cannot negotiate with yourself.";
        return false;
    }
    return true;
}
void Negotiate::execute(GameEngine* game) {
    if (*executed) return;
    if (!validate(game)) {
        *executed = true;
        return;
    }

    // Prevent attacks between issuer and target
    issuingPlayer->addDiplomaticAlly(targetPlayer);
    targetPlayer->addDiplomaticAlly(issuingPlayer);

    std::stringstream ss;
    ss << "Established diplomacy with " << targetPlayer->getName() << ". No attacks are allowed between "
        << issuingPlayer->getName() << " and " << targetPlayer->getName() << " for this turn.";
    *effect = ss.str();
    *executed = true;
}

//----------------------
//OrdersList
//----------------------
OrdersList::OrdersList() : orders(new std::vector<Order*>) {}
OrdersList::OrdersList(const OrdersList& other) : orders(new std::vector<Order*>) {
    orders->reserve(other.orders->size());
    for (Order* o : *other.orders) orders->push_back(o->clone());
}
OrdersList& OrdersList::operator=(const OrdersList& other) {
    if (this != &other) {
        for (Order* o : *orders) delete o;
        orders->clear();
        orders->reserve(other.orders->size());
        for (Order* o : *other.orders) orders->push_back(o->clone());
    }
    return *this;
}
OrdersList::~OrdersList() {
    for (Order* o : *orders) delete o;
    delete orders;
}
bool OrdersList::isFirstDeploy() const {
    if (orders->empty()) return false;
    return (orders->at(0)->getName() == "Deploy");
}
void OrdersList::add(Order* o) {
    if (o) orders->push_back(o);
}
void OrdersList::remove(size_t index) {
    if (index >= orders->size()) return;
    delete orders->at(index);
    orders->erase(orders->begin() + static_cast<long>(index));
}
void OrdersList::move(size_t from, size_t to) {
    if (from >= orders->size() || to >= orders->size() || from == to) return;
    Order* o = orders->at(from);
    orders->erase(orders->begin() + static_cast<long>(from));
    orders->insert(orders->begin() + static_cast<long>(to), o);
}
size_t OrdersList::size() const { return orders->size(); }
Order* OrdersList::at(size_t i) { return orders->at(i); }
const Order* OrdersList::at(size_t i) const { return orders->at(i); }

std::ostream& operator<<(std::ostream& out, const OrdersList& list) {
    out << "\nOrdersList (" << list.size() << " orders):";
    if (list.orders->empty()) {
        out << "[No Orders]";
        return out;
    }

    for (size_t i = 0; i < list.orders->size(); ++i) {
        out << *list.orders->at(i);
        if (i < list.orders->size() - 1) {
            out << ", ";
        }
    }
    return out;
}