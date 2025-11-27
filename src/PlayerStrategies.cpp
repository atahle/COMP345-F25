#include "PlayerStrategies.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h" 
#include "GameEngine.h"
#include <iostream>
#include <algorithm>
#include <random>
// Human Player

void HumanPlayerStrategy::issueOrder(Player* player, GameEngine* game) {
    vector<Territory*> defense = toDefend(player);
    vector<Territory*> attack = toAttack(player);

    // 1. DEPLOYMENT PHASE
    while (player->getReinforcementPool() > 0) {
        cout << "\n--- " << player->getName() << ": DEPLOYMENT (" << player->getReinforcementPool() << " remaining) --- \n";
        Territory* to = selectTerritory("Select territory to reinforce:", defense);
        if (!to) continue; // Failed to select, try again next loop

        int num = selectArmies("Enter number of armies to deploy:", player->getReinforcementPool());
        if (num <= 0) {
            cout << "Must deploy at least 1 army. Try again.\n";
            continue;
        }

        player->addOrder(new Deploy(player, to, num));
        player->addReinforcement(-num); // This is now temporary, execute() will pull from pool
        cout << "Deploy order issued. " << player->getReinforcementPool() << " reinforcements remain.\n";
    }

    // 2. MAIN ORDER ISSUING PHASE
    cout << "\n--- " << player->getName() << ": ORDER PHASE --- \n";
    cout << "Select an action:\n";
    cout << "  1. Advance (Attack/Move)\n";
    cout << "  2. Play a Card\n";
    cout << "  3. Pass (Finish turn)\n";
    cout << "> ";
    int choice;
    cin >> choice;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "Invalid input.\n";

    }
    while (true){

 
        switch (choice){
            // --- ADVANCE ---
        case 1:
        {
            cout << "Select action: 1. Attack 2. Move (Fortify)\n> ";
            cin >> choice;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cout << "Invalid input.\n";
            }

            Territory* from = selectTerritory("Select territory to move FROM:", defense);
            if (!from) break;

            Territory* to = nullptr;
            if (choice == 1) { // Attack
                to = selectTerritory("Select territory to ATTACK:", attack);
            }
            else { // Move
                to = selectTerritory("Select territory to move TO:", defense);
            }
            if (!to) break;

            int num = selectArmies("Enter number of armies to advance:", from->getArmyCount());
            if (num <= 0) {
                cout << "Must advance at least 1 army.\n";
                break;
            }

            player->addOrder(new Advance(player, from, to, num));
            cout << "Advance order issued.\n";
        }

        // --- PLAY CARD ---
        case 2:
        {
            if (player->getHand()->isEmpty()) {
                cout << "No cards in hand.\n";
                break;
            }
            cout << "Select a card to play:\n";
            cout << player->getHand() << "\n> ";
            int cardIdx;
            cin >> cardIdx;

            Card* selectedCard = player->getHand()->getCard(cardIdx - 1); // Peek at card
            if (!selectedCard) {
                cout << "Invalid card choice.\n";
                break;
            }

            Order* order = nullptr;
            switch (selectedCard->getType())
            {
            case Card::BOMB: { //
                cout << "BOMB: Select an enemy territory to bomb.\n";
                Territory* target = selectTerritory("Select target:", game->getAllTerritories()); // Show all
                if (!target) break;
                order = new Bomb(player, target);
                break;
            }
            case Card::BLOCKADE: { //
                cout << "BLOCKADE: Select one of your territories to blockade.\n";
                Territory* target = selectTerritory("Select target:", defense);
                if (!target) break;
                order = new Blockade(player, target);
                break;
            }
            case Card::AIRLIFT: { //
                cout << "AIRLIFT: Move armies between two of your territories.\n";
                Territory* from = selectTerritory("Select territory to airlift FROM:", defense);
                if (!from) break;
                Territory* to = selectTerritory("Select territory to airlift TO:", defense);
                if (!to) break;
                int num = selectArmies("Enter number of armies to airlift:", from->getArmyCount());
                order = new Airlift(player, from, to, num);
                break;
            }
            case Card::DIPLOMACY: { //
                cout << "DIPLOMACY: Select an enemy player to negotiate with.\n";
                Player* target = selectPlayer("Select target player:", game->getOtherPlayers(player));
                if (!target) break;
                order = new Negotiate(player, target);
                break;
            }
            case Card::REINFORCEMENT: {
                cout << "ERROR: Reinforcement card should not exist. (This is a deck-only card).\n";
                break;
            }
            } // end switch(card)

            if (order) {
                player->addOrder(order);
                player->getHand()->removeCard(cardIdx - 1); // Now remove card
                cout << "Card played and order issued.\n";
            }
            break;
        }

        // --- PASS ---
        case 3: {
            cout << player->getName() << " has finished issuing orders.\n";
            return; // Player is done for this turn
        }
        default:
            cout << "Invalid choice.\n";
            break;
        }
    }
}
std::vector<Territory*> HumanPlayerStrategy::toAttack(Player* player) {
    vector<Territory*> attackable;
    for (Territory* t : player->getTerritories())
    {
        for (Territory* adj : t->getAdjacentTerritories())
        {
            if (adj->getOwner() != player &&
                find(attackable.begin(), attackable.end(), adj) == attackable.end())
            {
                attackable.push_back(adj);
            }
        }
    }
    return attackable;
}
std::vector<Territory*> HumanPlayerStrategy::toDefend(Player* player) { return  player->getTerritories(); }

// Aggressive Player 
void AggressivePlayerStrategy::issueOrder(Player* player, GameEngine* game{
    std::cout << "[Aggressive] issueOrder called. Deploy/advance on strongest territory.\n";
}
std::vector<Territory*> AggressivePlayerStrategy::toAttack(Player* player) {
    std::vector<Territory*> attackList;
    
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
void BenevolentPlayerStrategy::issueOrder(Player* player, GameEngine* game) {

    vector<Territory*> owned = toDefend(player);

    Territory* weakest = owned.front();
    Territory* strongest = owned.back();



    //reinforcement phase
    if (player->getReinforcementPool() > 0) {
        int armies = player->getReinforcementPool();

        player->addOrder(new Deploy(player, weakest, armies));
        player->addReinforcement(-armies);
        cout << "[Benevolent] Deployed " << armies << " armies to " << weakest->getName() << ".\n";
        
    }



    if (std::rand() % 10 == 1) {

        Hand* hand = player->getHand();
        if (!hand->isEmpty()) {
            Card* cardToPlay = nullptr;
            int cardIdx = -1;

            for (int i = 0; i < hand->size(); i++) {
                Card::CardType type = hand->getCard(i)->getType();

                if (type == Card::BLOCKADE) {
                    cardToPlay = hand->getCard(i);
                    cardIdx = i;
                    player->addOrder(new Blockade(player, weakest));
                    cout << "[Benevolent] Playing BLOCKADE on " << weakest->getName() << ".\n";
                    break;
                }
                else if (type == Card::DIPLOMACY) {
                    vector<Player*> enemies = game->getOtherPlayers(player);
                    if (!enemies.empty()) {
                        cardToPlay = hand->getCard(i);
                        cardIdx = i;
                        // Simply negotiate with the first enemy in the list
                        player->addOrder(new Negotiate(player, enemies.at(0)));
                        cout << "[Benevolent] Playing DIPLOMACY with " << enemies.at(0)->getName() << ".\n";
                        break;
                    }
                }
                else if (type == Card::AIRLIFT) {
                    if (strongest->getArmyCount() > 1 && strongest != weakest) {
                        cardToPlay = hand->getCard(i);
                        cardIdx = i;
                        int armiesToMove = strongest->getArmyCount() / 2;
                        player->addOrder(new Airlift(player, strongest, weakest, armiesToMove));
                        cout << "[Benevolent] Playing AIRLIFT: " << armiesToMove << " armies from " << strongest->getName() << " to " << weakest->getName() << ".\n";
                        break;
                    }
                }
            }
        }
    }


    if (player->getOrders())

        for (int i = owned.size() - 1; i >= 0; i--) {
            Territory* strongT = owned[i];
            if (strongT->getArmyCount() < 2) continue;

            for (Territory* adj : strongT->getAdjacentTerritories()) {
                if (adj->getOwner() == player && adj->getArmyCount() < strongT->getArmyCount()) {

                    int transfer = (strongT->getArmyCount() - adj->getArmyCount()) / 2;
                    if (transfer > 0) {
                        player->addOrder(new Advance(player, strongT, adj, transfer));
                        cout << "[Benevolent] Fortifying " << adj->getName() << " with " << transfer << " armies from " << strongT->getName() << ".\n";
                            
                    }
                }
            }
        }

        cout << "[Benevolent] No suitable fortification moves. Ending turn.\n";
        return ;
  }

std::vector<Territory*> BenevolentPlayerStrategy::toAttack(Player* player) {
    // Benevolent never attacks
    return vector<Territory*>();
}

std::vector<Territory*> BenevolentPlayerStrategy::toDefend(Player* player) {
    vector<Territory*> owned = player->getTerritories();
    // Sort Ascending by army count (Weakest first)
    sort(owned.begin(), owned.end(), [](Territory* a, Territory* b) {
        return a->getArmyCount() < b->getArmyCount();
        });
    return owned;
}


// Neutral Player 
void NeutralPlayerStrategy::issueOrder(Player* player, GameEngine* game) {
    std::cout << "[Neutral] issueOrder called. Does nothing.\n";
    return;

}

std::vector<Territory*> NeutralPlayerStrategy::toAttack(Player* player) {
    return {};
}
std::vector<Territory*> NeutralPlayerStrategy::toDefend(Player* player) {
    return {};
}

// Cheater Player
void CheaterPlayerStrategy::issueOrder(Player* player, GameEngine* game) {
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

