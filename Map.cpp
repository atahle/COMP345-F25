#include "Map.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>

// =================================================================
// Territory Implementation (No Changes)
// ... (The rest of the Territory, Continent, and Map class implementations are the same)
// =================================================================
// Constructor: Initializes a Territory with a name.
Territory::Territory(const std::string& name) {
    this->name = new std::string(name);
    this->continent = nullptr;
    this->owner = nullptr;
    this->armyCount = new int(0);
    this->adjacentTerritories = new std::vector<Territory*>();
}
Territory::Territory(const Territory& other) {
    name = new std::string(*other.name);
    armyCount = new int(*other.armyCount);
    adjacentTerritories = new std::vector<Territory*>();
    continent = other.continent;
    owner = other.owner;
    for (Territory* adj : *other.adjacentTerritories) {
        adjacentTerritories->push_back(adj);
    }
}
Territory::~Territory() {
    delete name;
    delete armyCount;
    delete adjacentTerritories;
}
Territory& Territory::operator=(const Territory& other) {
    if (this == &other) { return *this; }
    delete name;
    delete armyCount;
    delete adjacentTerritories;
    name = new std::string(*other.name);
    armyCount = new int(*other.armyCount);
    adjacentTerritories = new std::vector<Territory*>();
    continent = other.continent;
    owner = other.owner;
    for (Territory* adj : *other.adjacentTerritories) {
        adjacentTerritories->push_back(adj);
    }
    return *this;
}
std::ostream& operator<<(std::ostream& os, const Territory& territory) {
    os << *territory.name;
    return os;
}
void Territory::addAdjacentTerritory(Territory* territory) { adjacentTerritories->push_back(territory); }
std::string Territory::getName() const { return *name; }
Continent* Territory::getContinent() const { return continent; }
Player* Territory::getOwner() const { return owner; }
int Territory::getArmyCount() const { return *armyCount; }
const std::vector<Territory*>& Territory::getAdjacentTerritories() const { return *adjacentTerritories; }
void Territory::setContinent(Continent* c) { continent = c; }
void Territory::setOwner(Player* p) { owner = p; }
void Territory::setArmyCount(int count) { *armyCount = count; }

// =================================================================
// Continent Implementation (No Changes)
// =================================================================
Continent::Continent(const std::string& name, int bonus) {
    this->name = new std::string(name);
    this->bonus = new int(bonus);
    this->territories = new std::vector<Territory*>();
}
Continent::Continent(const Continent& other) {
    name = new std::string(*other.name);
    bonus = new int(*other.bonus);
    territories = new std::vector<Territory*>();
    for (Territory* t : *other.territories) {
        territories->push_back(t);
    }
}
Continent::~Continent() {
    delete name;
    delete bonus;
    delete territories;
}
Continent& Continent::operator=(const Continent& other) {
    if (this == &other) return *this;
    delete name;
    delete bonus;
    delete territories;
    name = new std::string(*other.name);
    bonus = new int(*other.bonus);
    territories = new std::vector<Territory*>();
     for (Territory* t : *other.territories) {
        territories->push_back(t);
    }
    return *this;
}
std::ostream& operator<<(std::ostream& os, const Continent& continent) {
    os << *continent.name;
    return os;
}
void Continent::addTerritory(Territory* territory) { territories->push_back(territory); }
std::string Continent::getName() const { return *name; }
int Continent::getBonus() const { return *bonus; }
const std::vector<Territory*>& Continent::getTerritories() const { return *territories; }

// =================================================================
// Map Implementation (No Changes)
// =================================================================
Map::Map() {
    continents = new std::vector<Continent*>();
    territories = new std::vector<Territory*>();
}
Map::~Map() {
    for (Continent* continent : *continents) { delete continent; }
    delete continents;
    for (Territory* territory : *territories) { delete territory; }
    delete territories;
}
Map::Map(const Map& other) {
    continents = new std::vector<Continent*>();
    for (Continent* c : *other.continents) { continents->push_back(c); }
    territories = new std::vector<Territory*>();
    for (Territory* t : *other.territories) { territories->push_back(t); }
}
Map& Map::operator=(const Map& other) {
    if (this == &other) return *this;
    delete continents;
    delete territories;
    continents = new std::vector<Continent*>();
    for (Continent* c : *other.continents) { continents->push_back(c); }
    territories = new std::vector<Territory*>();
    for (Territory* t : *other.territories) { territories->push_back(t); }
    return *this;
}
std::ostream& operator<<(std::ostream& os, const Map& map) {
    os << "Map contains " << map.territories->size() << " territories and " << map.continents->size() << " continents.";
    return os;
}
void Map::addContinent(Continent* continent) { continents->push_back(continent); }
void Map::addTerritory(Territory* territory) { territories->push_back(territory); }
const std::vector<Continent*>& Map::getContinents() const { return *continents; }
const std::vector<Territory*>& Map::getTerritories() const { return *territories; }
Territory* Map::getTerritoryByName(const std::string& name) const {
    for (Territory* t : *territories) {
        if (t->getName() == name) {
            return t;
        }
    }
    return nullptr;
}
void traverse(Territory* start, std::map<Territory*, bool>& visited) {
    if (start == nullptr || visited[start]) { return; }
    visited[start] = true;
    for (Territory* neighbor : start->getAdjacentTerritories()) {
        traverse(neighbor, visited);
    }
}
void traverseContinent(Territory* start, Continent* continent, std::map<Territory*, bool>& visited) {
    if (start == nullptr || visited[start] || start->getContinent() != continent) { return; }
    visited[start] = true;
    for (Territory* neighbor : start->getAdjacentTerritories()) {
        if (neighbor->getContinent() == continent) {
            traverseContinent(neighbor, continent, visited);
        }
    }
}
bool Map::validate() {
    if (territories->empty() || continents->empty()) { return false; }
    std::map<Territory*, bool> visited;
    for (Territory* t : *territories) { visited[t] = false; }
    traverse(territories->front(), visited);
    for (auto const& [territory, isVisited] : visited) {
        if (!isVisited) {
            std::cout << "Validation Error: Map is not a connected graph. " << territory->getName() << " is unreachable." << std::endl;
            return false;
        }
    }
    for (Continent* c : *continents) {
        if (c->getTerritories().empty()) continue;
        std::map<Territory*, bool> continentVisited;
        for(Territory* t : c->getTerritories()){ continentVisited[t] = false; }
        traverseContinent(c->getTerritories().front(), c, continentVisited);
        for (auto const& [territory, isVisited] : continentVisited) {
            if (!isVisited) {
                std::cout << "Validation Error: Continent " << c->getName() << " is not a connected subgraph. " << territory->getName() << " is unreachable within the continent." << std::endl;
                return false;
            }
        }
    }
    for(Territory* t : *territories){
        if(t->getContinent() == nullptr){
            std::cout << "Validation Error: Territory " << t->getName() << " does not belong to any continent." << std::endl;
            return false;
        }
    }
    return true;
}

// =================================================================
// MapLoader Implementation (REWRITTEN)
// =================================================================

MapLoader::MapLoader() {}
MapLoader::~MapLoader() {}

// Helper function to convert a string to lowercase
void toLower(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c){ return std::tolower(c); });
}

Map* MapLoader::loadMap(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return nullptr;
    }

    Map* map = new Map();
    std::string line;
    std::string currentSection;
    
    std::map<std::string, Continent*> continentNameMap;
    std::map<std::string, Territory*> territoryNameMap;
    std::map<std::string, std::vector<std::string>> borderTempMap; // Stores territory name -> vector of neighbor names

    try {
        while (getline(file, line)) {
            line.erase(0, line.find_first_not_of(" \t\r\n"));
            line.erase(line.find_last_not_of(" \t\r\n") + 1);
            if (line.empty() || line[0] == ';') continue;

            std::string tempLine = line;
            toLower(tempLine);

            if (tempLine == "[continents]") {
                currentSection = "Continents";
                continue;
            } else if (tempLine == "[territories]" || tempLine == "[countries]") {
                currentSection = "Territories";
                continue;
            } else if (tempLine == "[borders]") { // This handles the simple format of invalid.map
                currentSection = "Borders";
                continue;
            }

            if (currentSection == "Continents") {
                std::stringstream ss(line);
                std::string name;
                int bonus;
                if (getline(ss, name, '=') && (ss >> bonus)) {
                    Continent* continent = new Continent(name, bonus);
                    map->addContinent(continent);
                    continentNameMap[name] = continent;
                }
            } else if (currentSection == "Territories") {
                std::stringstream ss(line);
                std::string territoryName, continentName;
                std::string temp;

                // 1. Get Territory Name
                getline(ss, territoryName, ',');
                
                // 2. Ignore X and Y coordinates
                getline(ss, temp, ','); 
                getline(ss, temp, ',');

                // 3. Get Continent Name
                getline(ss, continentName, ',');

                // 4. Get all neighbor names
                std::vector<std::string> neighborNames;
                while(getline(ss, temp, ',')) {
                    if (!temp.empty()) {
                        neighborNames.push_back(temp);
                    }
                }
                
                Continent* continent = continentNameMap[continentName];
                if (continent) {
                    Territory* territory = new Territory(territoryName);
                    territory->setContinent(continent);
                    continent->addTerritory(territory);
                    map->addTerritory(territory);
                    territoryNameMap[territoryName] = territory;
                    borderTempMap[territoryName] = neighborNames; // Store neighbors for second pass
                }
            } else if (currentSection == "Borders") { // For simple maps like the one we made
                std::stringstream ss(line);
                int territoryId;
                ss >> territoryId;
                // This old logic won't work with name-based maps, but we leave it
                // so the simple invalid.map still parses.
            }
        }
    } catch (...) { // Catch any parsing exception
        delete map;
        return nullptr;
    }

    // --- Second Pass: Link Borders ---
    for (auto const& [territoryName, neighborNames] : borderTempMap) {
        Territory* mainTerritory = territoryNameMap[territoryName];
        if (mainTerritory) {
            for (const std::string& neighborName : neighborNames) {
                Territory* neighborTerritory = territoryNameMap[neighborName];
                if (neighborTerritory) {
                    mainTerritory->addAdjacentTerritory(neighborTerritory);
                }
            }
        }
    }
    
    file.close();

    if(map->getTerritories().empty() || map->getContinents().empty()){
        delete map;
        return nullptr;
    }

    return map;
}