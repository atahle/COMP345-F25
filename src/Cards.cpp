#include "Cards.h"
#include <iostream>
#include <algorithm>
using namespace std;

// Card
Card::Card(CardType t) : type(t) {}

Card::Card(const Card& other) : type(other.type) {}

Card& Card::operator=(const Card& other) {
    if (this != &other) {
        type = other.type;
    }
    return *this;
}

Card::~Card() {}

Card::CardType Card::getType() const { return type; }

void Card::play() {
    string types[] = { "BOMB", "REINFORCEMENT", "BLOCKADE", "AIRLIFT", "DIPLOMACY" };
    cout << "Playing card: " << types[type] << endl;
}

ostream& operator<<(ostream& out, const Card& card) {
    string types[] = { "BOMB", "REINFORCEMENT", "BLOCKADE", "AIRLIFT", "DIPLOMACY" };
    out << types[card.type];
    return out;
}

// Deck
Deck::Deck(int numCards) {
    srand(time(0));
    for (int i = 0; i < numCards; i++) {
        Card::CardType type = static_cast<Card::CardType>(i % 5);
        cards.push_back(new Card(type));
    }
}

Deck::Deck(const Deck& other) {
    for (Card* c : other.cards) {
        cards.push_back(new Card(*c));
    }
}

Deck& Deck::operator=(const Deck& other) {
    if (this != &other) {
        for (Card* c : cards) delete c;
        cards.clear();
        for (Card* c : other.cards) cards.push_back(new Card(*c));
    }
    return *this;
}

Deck::~Deck() {
    for (Card* c : cards) delete c;
}

Card* Deck::draw() {
    if (cards.empty()) return nullptr;
    int index = rand() % cards.size();
    Card* c = cards[index];
    cards.erase(cards.begin() + index);
    return c;
}

void Deck::addCard(Card* card) {
    if (card) cards.push_back(card);
}

bool Deck::isEmpty() const { return cards.empty(); }

ostream& operator<<(ostream& out, const Deck& deck) {
    out << "Deck contains: ";
    for (Card* c : deck.cards) out << *c << " ";
    return out;
}

// Hand
Hand::Hand() {}

Hand::Hand(const Hand& other) {
    for (Card* c : other.hand) hand.push_back(new Card(*c));
}

Hand& Hand::operator=(const Hand& other) {
    if (this != &other) {
        for (Card* c : hand) delete c;
        hand.clear();
        for (Card* c : other.hand) hand.push_back(new Card(*c));
    }
    return *this;
}

Hand::~Hand() {
    for (Card* c : hand) delete c;
}

void Hand::addCard(Card* card) {
    if (card) hand.push_back(card);
}

Card* Hand::removeCard(int index) {
    if (index < 0 || index >= (int)hand.size()) return nullptr;
    Card* c = hand[index];
    hand.erase(hand.begin() + index);
    return c;
}

bool Hand::isEmpty() const { return hand.empty(); }

ostream& operator<<(ostream& out, const Hand& hand) {
    out << "Hand contains: ";
    for (Card* c : hand.hand) out << *c << " ";
    return out;
}
