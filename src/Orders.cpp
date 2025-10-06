#include "Orders.h"
#include <iomanip>

//Order
Order::Order(const std::string& n)
    : name(new std::string(n)), effect(new std::string("")), executed(false) {
}

Order::Order(const Order& other)
    : name(new std::string(*other.name)),
    effect(new std::string(*other.effect)),
    executed(other.executed) {
}

Order& Order::operator=(const Order& other) {
    if (this != &other) {
        *name = *other.name;
        *effect = *other.effect;
        executed = other.executed;
    }
    return *this;
}

Order::~Order() {
    delete name;
    delete effect;
}

bool Order::validate() const {
    return !executed; // can't re-execute
}

void Order::execute() {
    if (validate()) {
        executed = true;
        *effect = "Default action completed.";
    }
}

const std::string& Order::getName() const { return *name; }
const std::string& Order::getEffect() const { return *effect; }
bool Order::isExecuted() const { return executed; }

std::ostream& operator<<(std::ostream& out, const Order& o) {
    out << o.getName();
    if (o.isExecuted()) out << "\t| effect: " << o.getEffect();
    return out;
}

//Specific Orders
Deploy::Deploy() : Order("deploy") {}
Deploy* Deploy::clone() const { return new Deploy(*this); }
bool Deploy::validate() const { return Order::validate(); }
void Deploy::execute() {
    if (validate()) {
        executed = true;
        *effect = "Deployed reinforcement armies.";
    }
}

Advance::Advance() : Order("advance") {}
Advance* Advance::clone() const { return new Advance(*this); }
bool Advance::validate() const { return Order::validate(); }
void Advance::execute() {
    if (validate()) {
        executed = true;
        *effect = "Moved armies.";
    }
}

Bomb::Bomb() : Order("bomb") {}
Bomb* Bomb::clone() const { return new Bomb(*this); }
bool Bomb::validate() const { return Order::validate(); }
void Bomb::execute() {
    if (validate()) {
        executed = true;
        *effect = "Destroyed target armies.";
    }
}

Blockade::Blockade() : Order("blockade") {}
Blockade* Blockade::clone() const { return new Blockade(*this); }
bool Blockade::validate() const { return Order::validate(); }
void Blockade::execute() {
    if (validate()) {
        executed = true;
        *effect = "Positioned armies for blockade.";
    }
}

Airlift::Airlift() : Order("airlift") {}
Airlift* Airlift::clone() const { return new Airlift(*this); }
bool Airlift::validate() const { return Order::validate(); }
void Airlift::execute() {
    if (validate()) {
        executed = true;
        *effect = "Airlifted armies between territories.";
    }
}

Negotiate::Negotiate() : Order("negotiate") {}
Negotiate* Negotiate::clone() const { return new Negotiate(*this); }
bool Negotiate::validate() const { return Order::validate(); }
void Negotiate::execute() {
    if (validate()) {
        executed = true;
        *effect = "Established truce.";
    }
}

//OrdersList
OrdersList::OrdersList() : orders(new std::vector<Order*>) {}

OrdersList::OrdersList(const OrdersList& other) : orders(new std::vector<Order*>) {
    orders->reserve(other.orders->size());
    for (Order* o : *other.orders) orders->push_back(o->clone());
}

OrdersList& OrdersList::operator=(const OrdersList& other) {
    if (this != &other) {
        for (Order* o : *orders) delete o;
        orders->clear();
        orders->reserve(other.orders->size());
        for (Order* o : *other.orders) orders->push_back(o->clone());
    }
    return *this;
}

OrdersList::~OrdersList() {
    for (Order* o : *orders) delete o;
    delete orders;
}

void OrdersList::add(Order* o) {
    if (o) orders->push_back(o);
}

void OrdersList::remove(size_t index) {
    if (index >= orders->size()) return;
    delete orders->at(index);
    orders->erase(orders->begin() + static_cast<long>(index));
}

void OrdersList::move(size_t from, size_t to) {
    if (from >= orders->size() || to >= orders->size() || from == to) return;
    Order* o = orders->at(from);
    orders->erase(orders->begin() + static_cast<long>(from));
    orders->insert(orders->begin() + static_cast<long>(to), o);
}

size_t OrdersList::size() const { return orders->size(); }
const Order* OrdersList::at(size_t i) const { return orders->at(i); }

std::ostream& operator<<(std::ostream& out, const OrdersList& list) {
    out << "\nOrdersList:\n";
    for (size_t i = 0; i < list.size(); ++i) {
        out << "  - " << *list.at(i) << '\n';
    }
    return out;
}
