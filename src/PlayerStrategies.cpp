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
void AggressivePlayerStrategy::issueOrder(Player* player, GameEngine* game) {
    
    vector<Territory*> myTerritories = toDefend(player);

    
    Territory* strongest = myTerritories.front();

    
    int armiesToDeploy = player->getReinforcementPool();
    if (armiesToDeploy > 0) {
        player->addOrder(new Deploy(player, strongest, armiesToDeploy));
        player->addReinforcement(-armiesToDeploy); // Update pool
    }

    if (std::rand() % 10 == 1) {
        Hand* hand = player->getHand();
        if (!hand->isEmpty()) {
            for (int i = 0; i < hand->size(); ++i) {
                Card* card = hand->getCard(i);

                if (card->getType() == Card::BOMB) {
                    vector<Territory*> adjacent = strongest->getAdjacentTerritories();
                    for (Territory* adj : adjacent) {
                        if (adj->getOwner() != player) {
                            player->addOrder(new Bomb(player, adj));
                         
                            i--; 
                            break; 
                        }
                    }
                }
                else if (card->getType() == Card::AIRLIFT) {
                    // Airlift armies from the weakest territory to the strongest
                    Territory* weakest = myTerritories.back();
                    if (weakest != strongest && weakest->getArmyCount() > 0) {
                        int liftAmount = weakest->getArmyCount();
                        player->addOrder(new Airlift(player, weakest, strongest, liftAmount));
                        i--;
                        break;
                    }
                }
            }
        }
    }

    int totalArmies = strongest->getArmyCount() + armiesToDeploy;
    int moveableArmies = totalArmies - 1;

    if (moveableArmies > 0) {
        vector<Territory*> adjacent = strongest->getAdjacentTerritories();
        Territory* target = nullptr;

        // Try to find an enemy neighbor
        for (Territory* adj : adjacent) {
            if (adj->getOwner() != player) {
                target = adj;
                break; // Found an enemy to attack
            }
        }

        if (target != nullptr) {
            // Attack the enemy
            player->addOrder(new Advance(player, strongest, target, moveableArmies));
            cout << "[Aggressive] Advancing (Attacking) " << moveableArmies
                << " armies from " << strongest->getName() << " to " << target->getName() << ".\n";
        }
        else {
            // No enemy neighbors? Move armies to an adjacent friendly territory to get closer to action
            // (Simple logic: just move to the first neighbor)
            if (!adjacent.empty()) {
                target = adjacent[0];
                player->addOrder(new Advance(player, strongest, target, moveableArmies));
                cout << "[Aggressive] No enemies near strongest. Moving " << moveableArmies
                    << " armies to friendly " << target->getName() << ".\n";
            }
        }
    }
}

std::vector<Territory*> AggressivePlayerStrategy::toDefend(Player* player) {
    std::vector<Territory*> territories = player->getTerritories();

    std::sort(territories.begin(), territories.end(), [](Territory* a, Territory* b) {
        return a->getArmyCount() > b->getArmyCount();
        });

    return territories;
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


// Benevolent Player 
void BenevolentPlayerStrategy::issueOrder(Player* player, GameEngine* game) {

    vector<Territory*> owned = toDefend(player);

    Territory* weakest = owned.front();
    Territory* strongest = owned.back();



    //reinforcement phase
    while (player->getReinforcementPool() > 0) {
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
                    break;
                }
                else if (type == Card::DIPLOMACY) {
                    vector<Player*> enemies = game->getOtherPlayers(player);
                    if (!enemies.empty()) {
                        cardToPlay = hand->getCard(i);
                        cardIdx = i;
                        // Simply negotiate with the first enemy in the list
                        player->addOrder(new Negotiate(player, enemies.at(0)));
                        break;
                    }
                }
                else if (type == Card::AIRLIFT) {
                    if (strongest->getArmyCount() > 1 && strongest != weakest) {
                        cardToPlay = hand->getCard(i);
                        cardIdx = i;
                        int armiesToMove = strongest->getArmyCount() / 2;
                        player->addOrder(new Airlift(player, strongest, weakest, armiesToMove));
                        break;
                    }
                }
            }
        }
    }



        for (int i = static_cast<int>(owned.size()) - 1; i >= 0; i--) {
            Territory* strongT = owned[i];
            if (strongT->getArmyCount() < 2) continue;

            for (Territory* adj : strongT->getAdjacentTerritories()) {
                if (adj->getOwner() == player && adj->getArmyCount() < strongT->getArmyCount()) {
                    int transfer = (strongT->getArmyCount() - adj->getArmyCount()) / 2;
                    if (transfer > 0) {
                        player->addOrder(new Advance(player, strongT, adj, transfer));
                    }
                }
            }
        }

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
    
    if (player->getPrevTerritoryCount() > player->getTerritories().size()) {
        cout << "\n--- " << player->getName() << ": TRANSITION TO AGGRESSIVE ---\n";
        player->setStrategy(new AggressivePlayerStrategy());
        return;
    }
    else {
		player->setPrevTerritoryCount(player->getTerritories().size());
    }


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
    cout << "\n--- " << player->getName() << ": CHEATER TURN ---\n";

    vector<Territory*> owned = toDefend(player);
    vector<Territory*> toConquer = toAttack(player);


    sort(toConquer.begin(), toConquer.end());
    toConquer.erase(unique(toConquer.begin(), toConquer.end()), toConquer.end());

    for (Territory* target : toConquer) {
        cout << "[Cheater] Automatically conquering " << target->getName() << " from "
            << (target->getOwner() ? target->getOwner()->getName() : "Neutral") << ".\n";

        if (target->getOwner() != nullptr) {
            target->getOwner()->removeTerritory(target);
        }
        target->setOwner(player);
        player->addTerritory(target);
    }

}

std::vector<Territory*> CheaterPlayerStrategy::toAttack(Player* player) {
    vector<Territory*> targets;
    for (Territory* t : player->getTerritories()) {
        for (Territory* adj : t->getAdjacentTerritories()) {
            if (adj->getOwner() != player) {
                targets.push_back(adj);
            }
        }
    }
    return targets;
}

std::vector<Territory*> CheaterPlayerStrategy::toDefend(Player* player) {
    return player->getTerritories();
}

