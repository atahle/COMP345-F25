#ifndef ORDERS_H
#define ORDERS_H

#include <iostream>
#include <vector>
#include <string>

//Order
class Order {
protected:
    std::string* name;
    std::string* effect;
    bool executed;

public:
    Order(const std::string& n = "Order");
    Order(const Order& other);
    Order& operator=(const Order& other);
    virtual ~Order();

    virtual bool validate() const;
    virtual void execute();
    virtual Order* clone() const = 0;     
    const std::string& getName() const;
    const std::string& getEffect() const;
    bool isExecuted() const;

    // print description and effect
    friend std::ostream& operator<<(std::ostream& out, const Order& o);
};

//Specific Orders
class Deploy : public Order {
public:
    Deploy();
    Deploy* clone() const override;
    bool validate() const override;
    void execute() override;
};

class Advance : public Order {
public:
    Advance();
    Advance* clone() const override;
    bool validate() const override;
    void execute() override;
};

class Bomb : public Order {
public:
    Bomb();
    Bomb* clone() const override;
    bool validate() const override;
    void execute() override;
};

class Blockade : public Order {
public:
    Blockade();
    Blockade* clone() const override;
    bool validate() const override;
    void execute() override;
};

class Airlift : public Order {
public:
    Airlift();
    Airlift* clone() const override;
    bool validate() const override;
    void execute() override;
};

class Negotiate : public Order {
public:
    Negotiate();
    Negotiate* clone() const override;
    bool validate() const override;
    void execute() override;
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

    void add(Order* o);
    void remove(size_t index);
    void move(size_t from, size_t to);

    size_t size() const;
    const Order* at(size_t i) const;

    friend std::ostream& operator<<(std::ostream& out, const OrdersList& list);
};

//driver function
void testOrdersLists();

#endif // ORDERS_H
