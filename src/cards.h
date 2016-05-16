/**
 * Card related data structures and functions to handle this structures
 */

#ifndef CARDS_H
#define	CARDS_H


#include <stdbool.h>

/**
 * Enum type of suit
 */
typedef enum {CLUBS, DIAMONDS, HEARTS, SPADES, NUM_SUITS, BACK} Suit;

/**
 * Card is a ordinary struct
 */
typedef struct {
  int rank;
  Suit suit;
  int value;
} Card;

/**
 * Node for card lists
 */
typedef struct cardNode {
	Card card;
	struct cardNode *next;
} CardNode;


/**
 *	Pile is a linked list
 */
typedef struct {
	int numCards;
	int numDecks;
	CardNode *pileTop, *pileBottom;
} Pile;


Pile createPile();
void refillPile(Pile *cardPile);
CardNode *insertCardOnTop (Pile *pile, Card card);
CardNode *insertCardOnBottom (Pile *pile, Card card);
CardNode *dealCard(Pile *cardPile);
CardNode *removeCardFromTop(Pile *pile);
void listPile(Pile *pile);

#endif /* end include guard */
