#pragma once
#include <string>
#include <vector>
#include <iostream>

class Player;
class Territory;
class GameEngine;

//====================================================
// Order (Base Class)
//====================================================
class Order {
protected:
    std::string* name;
    std::string* effect;
    bool* executed;
    Player* issuingPlayer;

    // NEW: Polymorphic printing
    virtual void print(std::ostream& out) const;

public:
    Order(const std::string& n, Player* issuer);
    Order(const Order& other);
    Order& operator=(const Order& other);
    virtual ~Order();

    const std::string& getName() const;
    const std::string& getEffect() const;
    bool isExecuted() const;

    virtual bool validate(GameEngine* game) const = 0;
    virtual void execute(GameEngine* game) = 0;

    virtual Order* clone() const = 0;

    friend std::ostream& operator<<(std::ostream& out, const Order& o);
};

//====================================================
// Deploy
//====================================================
class Deploy : public Order {
private:
    Territory* targetTerritory;
    int* numArmies;

protected:
    void print(std::ostream& out) const override; // NEW

public:
    Deploy(Player* issuer, Territory* t, int numReinforcement);
    Deploy(const Deploy& other);
    ~Deploy();

    bool validate(GameEngine* game) const override;
    void execute(GameEngine* game) override;
    Deploy* clone() const override;
};

//====================================================
// Advance
//====================================================
class Advance : public Order {
private:
    Territory* sourceTerritory;
    Territory* targetTerritory;
    int* numArmies;

protected:
    void print(std::ostream& out) const override; // NEW

public:
    Advance(Player* issuer, Territory* from, Territory* to, int numReinforcement);
    Advance(const Advance& other);
    ~Advance();

    bool validate(GameEngine* game) const override;
    void execute(GameEngine* game) override;
    Advance* clone() const override;
};

//====================================================
// Bomb
//====================================================
class Bomb : public Order {
private:
    Territory* targetTerritory;

protected:
    void print(std::ostream& out) const override; // NEW

public:
    Bomb(Player* issuer, Territory* t);
    Bomb(const Bomb& other);
    ~Bomb();

    bool validate(GameEngine* game) const override;
    void execute(GameEngine* game) override;
    Bomb* clone() const override;
};

//====================================================
// Blockade
//====================================================
class Blockade : public Order {
private:
    Territory* targetTerritory;

protected:
    void print(std::ostream& out) const override; // NEW

public:
    Blockade(Player* issuer, Territory* t);
    Blockade(const Blockade& other);
    ~Blockade();

    bool validate(GameEngine* game) const override;
    void execute(GameEngine* game) override;
    Blockade* clone() const override;
};

//====================================================
// Airlift
//====================================================
class Airlift : public Order {
private:
    Territory* sourceTerritory;
    Territory* targetTerritory;
    int* numArmies;

protected:
    void print(std::ostream& out) const override; // NEW

public:
    Airlift(Player* issuer, Territory* from, Territory* to, int numArmies);
    Airlift(const Airlift& other);
    ~Airlift();

    bool validate(GameEngine* game) const override;
    void execute(GameEngine* game) override;
    Airlift* clone() const override;
};

//====================================================
// Negotiate
//====================================================
class Negotiate : public Order {
private:
    Player* targetPlayer;

protected:
    void print(std::ostream& out) const override; // NEW

public:
    Negotiate(Player* issuer, Player* target);
    Negotiate(const Negotiate& other);
    ~Negotiate();

    bool validate(GameEngine* game) const override;
    void execute(GameEngine* game) override;
    Negotiate* clone() const override;
};

//====================================================
// OrdersList
//====================================================
class OrdersList {
private:
    std::vector<Order*>* orders;

public:
    OrdersList();
    OrdersList(const OrdersList& other);
    OrdersList& operator=(const OrdersList& other);
    ~OrdersList();

    void add(Order* o);
    void remove(size_t index);
    void move(size_t from, size_t to);

    bool isFirstDeploy() const;

    size_t size() const;
    Order* at(size_t i);
    const Order* at(size_t i) const;

    friend std::ostream& operator<<(std::ostream& out, const OrdersList& list);
};

