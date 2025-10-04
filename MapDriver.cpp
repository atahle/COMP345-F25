#include "Map.h"
#include <iostream>
#include <vector>
#include <string>

// Forward declaration of the test function, as required by the assignment instructions
void testLoadMaps();

void testLoadMaps() {
    std::cout << "\n=========================================\n";
    std::cout << "        Testing Part 1: MapLoader\n";
    std::cout << "=========================================\n\n";

    MapLoader loader;

    // This list is now updated with the exact filenames you have.
    // Note that filenames with spaces like "Middle East Big.map" are handled correctly.
    std::vector<std::string> mapFilesToTest = {
        "Canada.map",
        "Earth.map",
        "Europe.map",
        "Middle East Big.map",
        "Quebec.map",
        "USA.map",
        "invalid.map",          // A custom invalid map to test rejection
        "non_existent_file.txt" // A file that doesn't exist to test error handling
    };

    // Loop through the list of files and try to load each one
    for (const std::string& filename : mapFilesToTest) {
        std::cout << "-----------------------------------------\n";
        std::cout << "Attempting to load map: " << filename << "\n";
        std::cout << "-----------------------------------------\n";

        Map* map = loader.loadMap(filename);

        // Check if the map was loaded successfully
        if (map != nullptr) {
            std::cout << "SUCCESS: Map loaded.\n";

            // If loaded, try to validate it
            std::cout << "Validating map...\n";
            if (map->validate()) {
                std::cout << "RESULT: Map is VALID. \n\n";
            } else {
                std::cout << "RESULT: Map is INVALID. \n\n";
            }

            // CRITICAL: Delete the map object to prevent memory leaks
            delete map;
            map = nullptr;
        } else {
            // This happens if the file doesn't exist or is fundamentally unreadable
            std::cout << "FAILURE: Map could not be loaded. File rejected. ❌\n\n";
        }
    }
}