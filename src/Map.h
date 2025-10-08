#pragma once

#include <string>
#include <vector>
#include <iostream>

// Forward declaration of classes to handle circular dependencies
class Territory;
class Continent;
class Player; // Required for Territory, will be fully implemented in Part 2

/**
 * @class Territory
 * @brief Represents a single territory (node) on the game map.
 * Each territory belongs to a continent, is owned by a player, and has a number of armies.
 */
class Territory {
public:
    // Constructors and Destructor
    Territory(const std::string& name);
    Territory(const Territory& other); // Copy Constructor
    ~Territory(); // Destructor

    // Operators
    Territory& operator=(const Territory& other); // Assignment Operator
    friend std::ostream& operator<<(std::ostream& os, const Territory& territory); // Stream Insertion

    // Methods
    void addAdjacentTerritory(Territory* territory);

    // Getters
    std::string getName() const;
    Continent* getContinent() const;
    Player* getOwner() const;
    int getArmyCount() const;
    const std::vector<Territory*>& getAdjacentTerritories() const;

    // Setters
    void setContinent(Continent* continent);
    void setOwner(Player* owner);
    void setArmyCount(int count);

private:
    std::string* name;
    Continent* continent;
    Player* owner;
    int* armyCount;
    std::vector<Territory*>* adjacentTerritories;
};

/**
 * @class Continent
 * @brief Represents a continent as a connected subgraph of territories.
 */
class Continent {
public:
    // Constructors and Destructor
    Continent(const std::string& name, int bonus);
    Continent(const Continent& other); // Copy Constructor
    ~Continent(); // Destructor

    // Operators
    Continent& operator=(const Continent& other); // Assignment Operator
    friend std::ostream& operator<<(std::ostream& os, const Continent& continent); // Stream Insertion

    // Methods
    void addTerritory(Territory* territory);

    // Getters
    std::string getName() const;
    int getBonus() const;
    const std::vector<Territory*>& getTerritories() const;

private:
    std::string* name;
    int* bonus;
    std::vector<Territory*>* territories;
};

/**
 * @class Map
 * @brief Represents the game map as a connected graph of territories and continents.
 */
class Map {
public:
    // Constructors and Destructor
    Map();
    Map(const Map& other); // Copy Constructor
    ~Map(); // Destructor

    // Operators
    Map& operator=(const Map& other); // Assignment Operator
    friend std::ostream& operator<<(std::ostream& os, const Map& map); // Stream Insertion

    // Methods
    void addContinent(Continent* continent);
    void addTerritory(Territory* territory);
    bool validate(); // Validates the map structure [cite: 48]

    // Getters
    const std::vector<Continent*>& getContinents() const;
    const std::vector<Territory*>& getTerritories() const;
    Territory* getTerritoryByName(const std::string& name) const;

private:
    std::vector<Continent*>* continents;
    std::vector<Territory*>* territories;
};

/**
 * @class MapLoader
 * @brief Responsible for loading a map file and creating a Map object.
 * Can read Conquest game map files. [cite: 36]
 */
class MapLoader {
public:
    MapLoader();
    MapLoader(const MapLoader& other);
    MapLoader& operator=(const MapLoader& other);
    ~MapLoader();

    Map* loadMap(const std::string& filename);

    friend std::ostream& operator<<(std::ostream& os, const MapLoader& loader);
};
