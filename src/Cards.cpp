#include "Cards.h"
#include <iostream>
#include <algorithm>
using namespace std;

// ---------------------- Card ----------------------

// Constructors and assignment operators
Card::Card(CardType t) : type(t) {}
Card::Card(const Card& other) : type(other.type) {}
Card::Card(Card&& other) noexcept : type(other.type) {}

Card& Card::operator=(const Card& other) {
    if (this != &other) type = other.type;
    return *this;
}

Card& Card::operator=(Card&& other) noexcept {
    if (this != &other) type = other.type;
    return *this;
}

Card::~Card() {}

// Returns the card type
Card::CardType Card::getType() const { return type; }

// Plays the card: prints actions and returns the card to the deck
void Card::play(Deck& deck) {
    string types[] = { "BOMB", "REINFORCEMENT", "BLOCKADE", "AIRLIFT", "DIPLOMACY" };

    cout << "Creating order from card: " << types[type] << endl;
    cout << "Order added to player's OrdersList." << endl;

    deck.addCard(new Card(type));
    cout << "Card returned to deck.\n";
}

// Prints the card type
ostream& operator<<(ostream& out, const Card& card) {
    string types[] = { "BOMB", "REINFORCEMENT", "BLOCKADE", "AIRLIFT", "DIPLOMACY" };
    out << types[card.type];
    return out;
}

// ---------------------- Deck ----------------------

// Creates a deck with a given number of cards (cyclic types)
Deck::Deck(int numCards) {
    srand(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < numCards; i++) {
        Card::CardType type = static_cast<Card::CardType>(i % 5);
        cards.push_back(new Card(type));
    }
}

// Copy constructor: deep copies all cards
Deck::Deck(const Deck& other) {
    for (Card* c : other.cards)
        cards.push_back(new Card(*c));
}

// Move constructor: transfers ownership
Deck::Deck(Deck&& other) noexcept : cards(move(other.cards)) {
    other.cards.clear();
}

// Copy assignment: deep copy
Deck& Deck::operator=(const Deck& other) {
    if (this != &other) {
        for (Card* c : cards) delete c;
        cards.clear();
        for (Card* c : other.cards)
            cards.push_back(new Card(*c));
    }
    return *this;
}

// Move assignment: transfers ownership
Deck& Deck::operator=(Deck&& other) noexcept {
    if (this != &other) {
        for (Card* c : cards) delete c;
        cards = move(other.cards);
        other.cards.clear();
    }
    return *this;
}

// Destructor: frees all cards
Deck::~Deck() {
    for (Card* c : cards) delete c;
}

// Draws a random card from the deck
Card* Deck::draw() {
    if (cards.empty()) {
        cout << "Deck is empty — cannot draw.\n";
        return nullptr;
    }
    int index = rand() % cards.size();
    Card* c = cards[index];
    cards.erase(cards.begin() + index);
    return c;
}

// Adds a card to the deck
void Deck::addCard(Card* card) {
    if (card) cards.push_back(card);
}

// Returns true if deck is empty
bool Deck::isEmpty() const { return cards.empty(); }

// Prints all cards in the deck
ostream& operator<<(ostream& out, const Deck& deck) {
    out << "Deck contains: ";
    for (Card* c : deck.cards)
        out << *c << " ";
    return out;
}

// ---------------------- Hand ----------------------

// Constructors and assignment
Hand::Hand() {}

Hand::Hand(const Hand& other) {
    for (Card* c : other.hand)
        hand.push_back(new Card(*c));
}

Hand::Hand(Hand&& other) noexcept : hand(move(other.hand)) {
    other.hand.clear();
}

Hand& Hand::operator=(const Hand& other) {
    if (this != &other) {
        for (Card* c : hand) delete c;
        hand.clear();
        for (Card* c : other.hand)
            hand.push_back(new Card(*c));
    }
    return *this;
}

Hand& Hand::operator=(Hand&& other) noexcept {
    if (this != &other) {
        for (Card* c : hand) delete c;
        hand = move(other.hand);
        other.hand.clear();
    }
    return *this;
}

// Destructor: frees all cards in the hand
Hand::~Hand() {
    for (Card* c : hand) delete c;
}

// Adds a card to the hand
void Hand::addCard(Card* card) {
    if (card) hand.push_back(card);
}

// Removes a card at the given index
Card* Hand::removeCard(int index) {
    if (index < 0 || index >= static_cast<int>(hand.size()))
        return nullptr;
    Card* c = hand[index];
    hand.erase(hand.begin() + index);
    return c;
}

// Returns true if hand is empty
bool Hand::isEmpty() const { return hand.empty(); }

// Prints all cards in the hand
ostream& operator<<(ostream& out, const Hand& hand) {
    out << "Hand contains: ";
    for (Card* c : hand.hand)
        out << *c << " ";
    return out;
}
