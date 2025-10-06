#ifndef CARDS_H
#define CARDS_H

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

class Deck; 

class Card {
public:
    enum CardType { BOMB, REINFORCEMENT, BLOCKADE, AIRLIFT, DIPLOMACY };

private:
    CardType type;

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

    CardType getType() const;

    // Play the card: creates an order, adds to list, and returns to deck
    void play(Deck& deck);

    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& out, const Card& card);
};

class Deck {
private:
    std::vector<Card*> cards;

public:
    Deck(int numCards = 20);
    Deck(const Deck& other);
    Deck(Deck&& other) noexcept;
    Deck& operator=(const Deck& other);
    Deck& operator=(Deck&& other) noexcept;
    ~Deck();

    Card* draw();
    void addCard(Card* card);
    bool isEmpty() const;

    friend std::ostream& operator<<(std::ostream& out, const Deck& deck);
};

class Hand {
private:
    std::vector<Card*> hand;

public:
    Hand();
    Hand(const Hand& other);
    Hand(Hand&& other) noexcept;
    Hand& operator=(const Hand& other);
    Hand& operator=(Hand&& other) noexcept;
    ~Hand();

    void addCard(Card* card);
    Card* removeCard(int index);
    bool isEmpty() const;

    friend std::ostream& operator<<(std::ostream& out, const Hand& hand);
};

#endif
