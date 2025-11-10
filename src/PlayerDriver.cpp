#include "Orders.h"
#include "Player.h"
#include "Map.h"
#include <iostream>
using namespace std;

// -----------------------------------------------------------
// Function: testPlayers
// Purpose : Demonstrates the functionality of the Player class.
//           Tests construction, copy, assignment, territory
//           management, order issuing, and list retrieval.
// -----------------------------------------------------------
void testPlayers()
{
    cout << "\n===================================" << endl;
    cout << "        TESTING PLAYER CLASS       " << endl;
    cout << "===================================\n" << endl;

    // Create a player
    Player p1("Alisson");
    cout << "[1] Created Player:" << endl;
    cout << p1 << "\n" << endl;

    // Create some sample territories
    Territory t1("Territory1");
    Territory t2("Territory2");

    // Add territories to player
    p1.addTerritory(&t1);
    p1.addTerritory(&t2);
    cout << "[2] After adding territories:" << endl;
    cout << p1 << "\n" << endl;

    // Remove one territory
    p1.removeTerritory(&t1);
    cout << "[3] After removing a territory:" << endl;
    cout << p1 << "\n" << endl;

    // Copy constructor test
    Player p2 = p1;
    cout << "[4] Copied Player (p2):" << endl;
    cout << p2 << "\n" << endl;

    // Assignment operator test
    Player p3("Robert");
    p3 = p1;
    cout << "[5] Assigned Player (p3):" << endl;
    cout << p3 << "\n" << endl;

    // Show territories to defend
    cout << "[6] Territories to defend for p1:" << endl;
    for (Territory* t : p1.toDefend())
        cout << " - " << t->getName() << endl;
    cout << endl;

    // Show territories to attack
    cout << "[7] Territories to attack for p1:" << endl;
    for (Territory* t : p1.toAttack())
        cout << " - " << t->getName() << endl;
    cout << endl;

    // Issue some orders and print them
    //p1.issueOrder("deploy");
    //p1.issueOrder("advance");
    cout << "[8] After issuing orders to p1:" << endl;
    cout << p1 << "\n" << endl;

    cout << "[9] OrdersList contents:" << endl;
    cout << p1.getOrders() << "\n" << endl;

    cout << "===================================" << endl;
    cout << "      END OF PLAYER CLASS TESTS    " << endl;
    cout << "===================================\n" << endl;
}
