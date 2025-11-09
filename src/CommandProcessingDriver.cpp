#include "CommandProcessing.h"
#include <iostream>
#include <limits>

// ---------------------- Driver ----------------------
static void runDemo(CommandProcessor& cp, GameEngine& ge, int maxSteps = 20) {
    cp.attachEngine(&ge);

    for (int i = 0; i < maxSteps; ++i) {
        Command* c = cp.getCommand();
        if (!c) break;
        std::cout << *c << "\n";
        if (c->getRaw() == "quit") break; // optional stop
    }

    std::cout << "\n" << cp << "\n";
}

void testCommandProcessor() {
    std::cout << "\n=== testCommandProcessor (A2 Part 1) ===\n";
    GameEngine ge;
    ge.play(); // Start

    std::cout << "Choose mode:\n";
    std::cout << "  - Type '-console' for console input\n";
    std::cout << "  - Or type '-file <path>' to execute a file of commands\n> ";

    std::string modeLine;
    std::getline(std::cin, modeLine);

    // Basic parse: treat any line starting with "-file" as file mode
    if (modeLine.rfind("-file", 0) == 0) {
        std::string filename = modeLine.substr(5);
        // trim leading spaces
        size_t p = filename.find_first_not_of(" \t");
        if (p != std::string::npos) filename = filename.substr(p);

        FileCommandProcessorAdapter fcp(filename);
        std::cout << "[File mode] Reading from: " << filename << "\n";
        runDemo(fcp, ge);
    } else {
        CommandProcessor cp;
        std::cout << "[Console mode] Enter commands (one per line). Examples:\n"
                     "  loadmap Canada.map\n"
                     "  validatemap\n"
                     "  addplayer Alice\n"
                     "  addplayer Bob\n"
                     "  gamestart\n";
        runDemo(cp, ge);
    }

    std::cout << "=== end testCommandProcessor ===\n";
}
