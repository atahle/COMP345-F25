#include <iostream>

// Forward declaration for the test function defined in MapDriver.cpp.
// This tells the compiler that the function exists and will be linked later.
void testLoadMaps();

int main() {
    std::cout << "🚀 Starting Warzone Game C++ Project Tests 🚀\n";

    // Call the test function for Part 1
    testLoadMaps();

    // In the future, you will add calls to other test functions here:
    // testPlayers();
    // testOrdersLists();
    // testCards();
    // testGameStates();

    std::cout << "\nAll tests have concluded. Exiting program.\n";

    return 0;
}