#include "Cards.h"
#include <iostream>
using namespace std;

// -----------------------------------------------------------
// Function: testCards
// Purpose : Demonstrates the functionality of the Card, Deck,
//           and Hand classes. Tests drawing, playing, and 
//           returning cards to the deck.
// -----------------------------------------------------------
void testCards() {
    cout << "=== Testing Cards, Deck, and Hand ===" << endl;

    // Create a deck with 10 cards
    Deck deck(10);
    Hand hand;

    // Draw 5 cards from the deck and add them to the hand
    for (int i = 0; i < 5; i++) {
        Card* card = deck.draw();
        if (card) hand.addCard(card);
    }

    // Show the hand before playing
    cout << "\nHand before playing:" << endl;
    cout << hand << endl;

    // Play all cards in the hand
    while (!hand.isEmpty()) {
        Card* card = hand.removeCard(0);  // Remove top card
        card->play(deck);                 // Play it and return it to the deck
        delete card;                      // Delete local copy (deck keeps its own)
    }

    // Show the hand after all cards have been played
    cout << "\nHand after playing (should be empty):" << endl;
    cout << hand << endl;

    // Show deck contents after all cards returned
    cout << "\nDeck after all cards returned:" << endl;
    cout << deck << endl;

    cout << "\n=== Done Testing ===" << endl;
}
