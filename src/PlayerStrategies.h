#ifndef PLAYERSTRATEGIES_H
#define PLAYERSTRATEGIES_H

#include <vector>
#include <iostream>
#include "Player.h"
#include "Map.h"

// Abstract Class
class PlayerStrategy {
public:
    virtual ~PlayerStrategy() = default;

    // Issue orders for the player
    virtual void issueOrder(Player* player) = 0;

    // Return list of territories the player wants to attack
    virtual std::vector<Territory*> toAttack(Player* player) = 0;

    // Return list of territories the player wants to defend
    virtual std::vector<Territory*> toDefend(Player* player) = 0;
};

// Human Player Strategy 
class HumanPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Player* player) override;
    std::vector<Territory*> toAttack(Player* player) override;
    std::vector<Territory*> toDefend(Player* player) override;
};

// Aggressive Player Strategy 
class AggressivePlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Player* player) override;
    std::vector<Territory*> toAttack(Player* player) override;
    std::vector<Territory*> toDefend(Player* player) override;
};

// Benevolent Player Strategy 
class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Player* player) override;
    std::vector<Territory*> toAttack(Player* player) override;
    std::vector<Territory*> toDefend(Player* player) override;
};

// Neutral Player Strategy 
class NeutralPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Player* player) override;
    std::vector<Territory*> toAttack(Player* player) override;
    std::vector<Territory*> toDefend(Player* player) override;

    // Called if Neutral player is attacked
    void becomeAggressive(Player* player);
};

// Cheater Player Strategy
class CheaterPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Player* player) override;
    std::vector<Territory*> toAttack(Player* player) override;
    std::vector<Territory*> toDefend(Player* player) override;
};

#endif // PLAYERSTRATEGIES_H
