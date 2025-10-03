#include "Orders.h"
#include <iostream>

void testOrdersLists() {
    std::cout << "\n--- Orders List Test ---\n\n";

    OrdersList list;
    list.add(new Deploy());
    list.add(new Advance());
    list.add(new Bomb());
    list.add(new Blockade());
    list.add(new Airlift());
    list.add(new Negotiate());

    std::cout << "Initial list: " << list << std::endl;

    list.move(0, list.size() - 1);
    std::cout << "After move(0 -> last):" << list << std::endl;

    list.remove(2);
    std::cout << "After remove(2):" << list << std::endl;

    for (size_t i = 0; i < list.size(); ++i)
        const_cast<Order*>(list.at(i))->execute();

    std::cout << "After execute all:" << list << std::endl;
    std::cout << "\n------------------\n";
}
int main() {
    testOrdersLists();
    return 0;
}