#ifndef CARDS_H
#define CARDS_H

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

class Card {
public:
    enum CardType { BOMB, REINFORCEMENT, BLOCKADE, AIRLIFT, DIPLOMACY };

private:
    CardType type;

public:
    // Constructor
    Card(CardType t);

    // Copy constructor
    Card(const Card& other);

    // Assignment operator
    Card& operator=(const Card& other);

    // Destructor
    ~Card();

    CardType getType() const;

    // Play the card: returns a string representing the action
    void play();

    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& out, const Card& card);
};

class Deck {
private:
    std::vector<Card*> cards;

public:
    Deck(int numCards = 20);  // default deck size
    Deck(const Deck& other);
    Deck& operator=(const Deck& other);
    ~Deck();

    Card* draw();       // draw a random card from the deck
    void addCard(Card* card); // add card back to deck
    bool isEmpty() const;

    friend std::ostream& operator<<(std::ostream& out, const Deck& deck);
};

class Hand {
private:
    std::vector<Card*> hand;

public:
    Hand();
    Hand(const Hand& other);
    Hand& operator=(const Hand& other);
    ~Hand();

    void addCard(Card* card);
    Card* removeCard(int index);
    bool isEmpty() const;

    friend std::ostream& operator<<(std::ostream& out, const Hand& hand);
};

#endif
