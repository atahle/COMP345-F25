#include "PlayerStrategies.h"

// Human Player
void HumanPlayerStrategy::issueOrder(Player* player) {
    std::cout << "[Human] issueOrder called. Please enter your move (deploy/advance/card): \n";
}
std::vector<Territory*> HumanPlayerStrategy::toAttack(Player* player) {
    std::cout << "[Human] toAttack called. Decide which territories to attack.\n";
    return {};
}
std::vector<Territory*> HumanPlayerStrategy::toDefend(Player* player) {
    std::cout << "[Human] toDefend called. Decide which territories to defend.\n";
    return {};
}

// Aggressive Player 
void AggressivePlayerStrategy::issueOrder(Player* player) {
    std::cout << "[Aggressive] issueOrder called. Deploy/advance on strongest territory.\n";
}
std::vector<Territory*> AggressivePlayerStrategy::toAttack(Player* player) {
    std::vector<Territory*> attackList;
    // Pick strongest territory and its adjacent enemy territories
    int maxArmy = -1;
    Territory* strongest = nullptr;
    for (auto t : player->getTerritories()) {
        if (t->getArmyCount() > maxArmy) {
            maxArmy = t->getArmyCount();
            strongest = t;
        }
    }
    if (strongest) {
        for (auto adj : strongest->getAdjacentTerritories()) {
            if (adj->getOwner() != player) {
                attackList.push_back(adj);
            }
        }
    }
    return attackList;
}
std::vector<Territory*> AggressivePlayerStrategy::toDefend(Player* player) {
    std::vector<Territory*> defendList;
    // Defend the strongest territory
    int maxArmy = -1;
    Territory* strongest = nullptr;
    for (auto t : player->getTerritories()) {
        if (t->getArmyCount() > maxArmy) {
            maxArmy = t->getArmyCount();
            strongest = t;
        }
    }
    if (strongest) defendList.push_back(strongest);
    return defendList;
}

// Benevolent Player
void BenevolentPlayerStrategy::issueOrder(Player* player) {
    std::cout << "[Benevolent] issueOrder called. Reinforce weakest territories.\n";
}
std::vector<Territory*> BenevolentPlayerStrategy::toAttack(Player* player) {
    return {}; // Never attacks
}
std::vector<Territory*> BenevolentPlayerStrategy::toDefend(Player* player) {
    std::vector<Territory*> defendList;
    // Defend weakest territories
    int minArmy = INT_MAX;
    for (auto t : player->getTerritories()) {
        if (t->getArmyCount() < minArmy) minArmy = t->getArmyCount();
    }
    for (auto t : player->getTerritories()) {
        if (t->getArmyCount() == minArmy) defendList.push_back(t);
    }
    return defendList;
}

// Neutral Player 
void NeutralPlayerStrategy::issueOrder(Player* player) {
    std::cout << "[Neutral] issueOrder called. Does nothing.\n";
}
std::vector<Territory*> NeutralPlayerStrategy::toAttack(Player* player) {
    return {};
}
std::vector<Territory*> NeutralPlayerStrategy::toDefend(Player* player) {
    return {};
}

// Cheater Player
void CheaterPlayerStrategy::issueOrder(Player* player) {
    std::cout << "[Cheater] issueOrder called. Conquer adjacent territories.\n";
    std::vector<Territory*> newTerritories;
    for (auto t : player->getTerritories()) {
        for (auto adj : t->getAdjacentTerritories()) {
            if (adj->getOwner() != player) {
                adj->setOwner(player);
                newTerritories.push_back(adj);
                std::cout << "Cheater conquered: " << adj->getName() << "\n";
            }
        }
    }

    for (auto t : newTerritories) player->addTerritory(t);
}
std::vector<Territory*> CheaterPlayerStrategy::toAttack(Player* player) {
    return {}; 
}
std::vector<Territory*> CheaterPlayerStrategy::toDefend(Player* player) {
    return player->getTerritories();
}

