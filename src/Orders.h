#ifndef ORDERS_H
#define ORDERS_H

#include <iostream>
#include <vector>
#include <string>

// Forward declarations to avoid circular dependencies
class Territory;
class Player;
class GameEngine;

//Order
class Order {
protected:
    std::string* name;
    std::string* effect;
    bool* executed;
    Player* issuingPlayer; // All orders need to know who issued them

public:
    Order(const std::string& n, Player* issuer);
    Order(const Order& other);
    Order& operator=(const Order& other);
    virtual ~Order();

    virtual bool validate(GameEngine* game) const = 0; // Pure virtual
    virtual void execute(GameEngine* game) = 0;  // Pure virtual
    virtual Order* clone() const = 0;

    const std::string& getName() const;
    const std::string& getEffect() const;
    bool isExecuted() const;

    // print description and effect
    friend std::ostream& operator<<(std::ostream& out, const Order& o);
};

//Specific Orders
class Deploy : public Order { //
private:
    Territory* targetTerritory;
    int* numArmies;
public:
    Deploy(Player* issuer, Territory* t, int numReinforcement);
    Deploy(const Deploy& other);
    ~Deploy();
    Deploy* clone() const override;
    bool validate(GameEngine* game) const override;
    void execute(GameEngine* game) override;
};

class Advance : public Order { //
private:
    Territory* sourceTerritory;
    Territory* targetTerritory;
    int* numArmies;
public:
    Advance(Player* issuer, Territory* from, Territory* to, int numReinforcement);
    Advance(const Advance& other);
    ~Advance();
    Advance* clone() const override;
    bool validate(GameEngine* game) const override;
    void execute(GameEngine* game) override;
};

class Bomb : public Order { //
private:
    Territory* targetTerritory;
public:
    Bomb(Player* issuer, Territory* t);
    Bomb(const Bomb& other);
    ~Bomb();
    Bomb* clone() const override;
    bool validate(GameEngine* game) const override;
    void execute(GameEngine* game) override;
};

class Blockade : public Order { //
private:
    Territory* targetTerritory;
public:
    Blockade(Player* issuer, Territory* t);
    Blockade(const Blockade& other);
    ~Blockade();
    Blockade* clone() const override;
    bool validate(GameEngine* game) const override;
    void execute(GameEngine* game) override;
};

class Airlift : public Order { //
private:
    Territory* sourceTerritory;
    Territory* targetTerritory;
    int* numArmies;
public:
    Airlift(Player* issuer, Territory* from, Territory* to, int numArmies);
    Airlift(const Airlift& other);
    ~Airlift();
    Airlift* clone() const override;
    bool validate(GameEngine* game) const override;
    void execute(GameEngine* game) override;
};

class Negotiate : public Order { //
private:
    Player* targetPlayer;
public:
    Negotiate(Player* issuer, Player* target);
    Negotiate(const Negotiate& other);
    ~Negotiate();
    Negotiate* clone() const override;
    bool validate(GameEngine* game) const override;
    void execute(GameEngine* game) override;
};

//OrdersList
class OrdersList {
private:
    std::vector<Order*>* orders; //pointers container
public:
    OrdersList();
    OrdersList(const OrdersList& other);
    OrdersList& operator=(const OrdersList& other);
    ~OrdersList();

    bool isFirstDeploy() const;
    void add(Order* o);
    void remove(size_t index);
    void move(size_t from, size_t to);

    size_t size() const;
    Order* at(size_t i); // Changed to non-const
    const Order* at(size_t i) const; // Overloaded const version

    friend std::ostream& operator<<(std::ostream& out, const OrdersList& list);
};

//driver function
void testOrdersLists();
void testOrderExecution(); // Part 4 Driver

#endif // ORDERS_H