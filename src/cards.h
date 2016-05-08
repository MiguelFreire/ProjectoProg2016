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
	CardNode *pileTop, *pileBottom;
} Pile;

Pile createPile();


bool pileIsEmpty (Pile pile);

void refillPile(Pile *cardPile, int numDecks);

CardNode *dealCard(Pile *cardPile);

void listPile(Pile pile);

CardNode *insertCardOnTop (CardNode *top, Card card);

CardNode *insertCardOnBottom (CardNode *top, Card card);


#endif /* end include guard */
