#include "Cards.h"
#include <iostream>
using namespace std;

void testCards() {
    cout << "=== Testing Cards, Deck, and Hand ===" << endl;

    // create a deck of 10 cards
    Deck deck(10);   
    Hand hand;

    // Draw 5 cards into the hand
    for (int i = 0; i < 5; i++) {
        Card* card = deck.draw();
        if (card) hand.addCard(card);
    }

    cout << "\nHand before playing:" << endl;
    cout << hand << endl;

    // Play all cards
    while (!hand.isEmpty()) {
        Card* card = hand.removeCard(0);
        card->play();     // simulate playing
        deck.addCard(card); // return card to deck
        delete card;      // free memory
    }

    cout << "\nHand after playing (should be empty):" << endl;
    cout << hand << endl;

    cout << "=== Done Testing ===" << endl;
}

// Main for testing
int main() {
    testCards();
    return 0;
}
