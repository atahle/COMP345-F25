#include "Cards.h"
#include <iostream>
using namespace std;

void testCards() {
    cout << "=== Testing Cards, Deck, and Hand ===" << endl;

    // Create a deck of 10 cards
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
        card->play(deck);  // Takes Deck reference and returns to it
        delete card;       // Deck holds a copy (not the same pointer)
    }

    cout << "\nHand after playing (should be empty):" << endl;
    cout << hand << endl;

    cout << "\nDeck after all cards returned:" << endl;
    cout << deck << endl;

    cout << "\n=== Done Testing ===" << endl;
}
