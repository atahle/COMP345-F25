#ifndef PLAYERSTRATEGIES_H
#define PLAYERSTRATEGIES_H

#include <vector>
#include <iostream>
#include "Player.h"
#include "Territory.h"

// Abstract Class
class PlayerStrategy {
public:
    virtual ~PlayerStrategy() = default;

    virtual void issueOrder(Player* player) = 0;
    virtual std::vector<Territory*> toAttack(Player* player) = 0;
    virtual std::vector<Territory*> toDefend(Player* player) = 0;
};

// Concrete Strategies 

// Human Player
class HumanPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Player* player) override;
    std::vector<Territory*> toAttack(Player* player) override;
    std::vector<Territory*> toDefend(Player* player) override;
};

// Aggressive Player
class AggressivePlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Player* player) override;
    std::vector<Territory*> toAttack(Player* player) override;
    std::vector<Territory*> toDefend(Player* player) override;
};

// Benevolent Player
class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Player* player) override;
    std::vector<Territory*> toAttack(Player* player) override;
    std::vector<Territory*> toDefend(Player* player) override;
};

// Neutral Player
class NeutralPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Player* player) override;
    std::vector<Territory*> toAttack(Player* player) override;
    std::vector<Territory*> toDefend(Player* player) override;
};

// Cheater Player
class CheaterPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Player* player) override;
    std::vector<Territory*> toAttack(Player* player) override;
    std::vector<Territory*> toDefend(Player* player) override;
};

#endif // PLAYERSTRATEGIES_H
