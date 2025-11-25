#include "PlayerStrategies.h"

// Human Player
void HumanPlayerStrategy::issueOrder(Player* player) {
    std::cout << "[Human] issueOrder called. Prompt user for deploy/advance/card.\n";
}
std::vector<Territory*> HumanPlayerStrategy::toAttack(Player* player) { return {}; }
std::vector<Territory*> HumanPlayerStrategy::toDefend(Player* player) { return {}; }

// Aggressive Player 
void AggressivePlayerStrategy::issueOrder(Player* player) {
    std::cout << "[Aggressive] issueOrder called. Attack strongest territory.\n";
}
std::vector<Territory*> AggressivePlayerStrategy::toAttack(Player* player) { return {}; }
std::vector<Territory*> AggressivePlayerStrategy::toDefend(Player* player) { return {}; }

// Benevolent Player 
void BenevolentPlayerStrategy::issueOrder(Player* player) {
    std::cout << "[Benevolent] issueOrder called. Reinforce weakest territories.\n";
}
std::vector<Territory*> BenevolentPlayerStrategy::toAttack(Player* player) { return {}; }
std::vector<Territory*> BenevolentPlayerStrategy::toDefend(Player* player) { return {}; }

// Neutral Player 
void NeutralPlayerStrategy::issueOrder(Player* player) {
    std::cout << "[Neutral] issueOrder called. Does nothing.\n";
}
std::vector<Territory*> NeutralPlayerStrategy::toAttack(Player* player) { return {}; }
std::vector<Territory*> NeutralPlayerStrategy::toDefend(Player* player) { return {}; }

//  Cheater Player 
void CheaterPlayerStrategy::issueOrder(Player* player) {
    std::cout << "[Cheater] issueOrder called. Conquer adjacent territories automatically.\n";
}
std::vector<Territory*> CheaterPlayerStrategy::toAttack(Player* player) { return {}; }
std::vector<Territory*> CheaterPlayerStrategy::toDefend(Player* player) { return {}; }
