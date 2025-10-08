#ifndef CARDS_H
#define CARDS_H

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

class Deck; // Forward declaration

// ---------------------- Card ----------------------
// Represents a single card that can create an order
class Card {
public:
    enum CardType { BOMB, REINFORCEMENT, BLOCKADE, AIRLIFT, DIPLOMACY };

private:
    CardType type; // Type of the card

public:
    // Constructors
    Card(CardType t);
    Card(const Card& other);
    Card(Card&& other) noexcept;

    // Assignment operators
    Card& operator=(const Card& other);
    Card& operator=(Card&& other) noexcept;

    // Destructor
    ~Card();

    // Returns the type of card
    CardType getType() const;

    // Plays the card (creates an order and returns it to the deck)
    void play(Deck& deck);

    // Prints the card type
    friend std::ostream& operator<<(std::ostream& out, const Card& card);
};

// ---------------------- Deck ----------------------
// Holds and manages a collection of cards
class Deck {
private:
    std::vector<Card*> cards; // List of card pointers

public:
    // Constructors and destructor
    Deck(int numCards = 20);
    Deck(const Deck& other);
    Deck(Deck&& other) noexcept;
    Deck& operator=(const Deck& other);
    Deck& operator=(Deck&& other) noexcept;
    ~Deck();

    // Draws and removes a random card from the deck
    Card* draw();

    // Adds a card back to the deck
    void addCard(Card* card);

    // Checks if the deck is empty
    bool isEmpty() const;

    // Prints all cards in the deck
    friend std::ostream& operator<<(std::ostream& out, const Deck& deck);
};

// ---------------------- Hand ----------------------
// Represents the player’s hand of cards
class Hand {
private:
    std::vector<Card*> hand; // List of card pointers in the player's hand

public:
    // Constructors and destructor
    Hand();
    Hand(const Hand& other);
    Hand(Hand&& other) noexcept;
    Hand& operator=(const Hand& other);
    Hand& operator=(Hand&& other) noexcept;
    ~Hand();

    // Adds a card to the hand
    void addCard(Card* card);

    // Removes a card at a specific index
    Card* removeCard(int index);

    // Checks if the hand is empty
    bool isEmpty() const;

    // Prints all cards in the hand
    friend std::ostream& operator<<(std::ostream& out, const Hand& hand);
};

#endif
