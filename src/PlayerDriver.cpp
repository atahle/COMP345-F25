#include "Orders.h" 
#include "Player.h"
#include "Map.h"
#include <iostream>


void testPlayers()
{
    std::cout << "----- Testing Player Class -----" << std::endl;

    Player p1("Alisson");
    std::cout << "Created Player: " << p1 << std::endl;

    Territory t1("Territory1");
    Territory t2("Territory2");
    p1.addTerritory(&t1);
    p1.addTerritory(&t2);
    std::cout << "After adding territories: " << p1 << std::endl;

    p1.removeTerritory(&t1);
    std::cout << "After removing a territory: " << p1 << std::endl;

    Player p2 = p1;
    std::cout << "Copied Player (p2): " << p2 << std::endl;

    Player p3("Robert");
    p3 = p1;
    std::cout << "Assigned Player (p3): " << p3 << std::endl;

    std::cout << "Territories to defend for p1: ";
    for (Territory *t : p1.toDefend())
    {
        std::cout << t->getName() << " " << endl;
    }

    std::cout << "\nTerritories to attack for p1: ";
    for (Territory *t : p1.toAttack())
    {
        std::cout << t->getName() << " " << endl;
    }

    p1.issueOrder("deploy");
    p1.issueOrder("advance");
    std::cout << "After issuing orders to p1: " << p1 << std::endl;
    std::cout << p1.getOrders() << std::endl;

    std::cout << "----- End of Player Class Tests -----" << std::endl;
}
