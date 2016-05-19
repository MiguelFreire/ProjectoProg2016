////////////////////////////////////////////////////////////////////////////////
//									CARDS.H									  //
//																			  //
// Card structures and functions to manipulate the card pile				  //
////////////////////////////////////////////////////////////////////////////////

#ifndef CARDS_H
#define	CARDS_H


#include <stdbool.h>

/**
 * Enum type of suit
 */
typedef enum {CLUBS, 
	DIAMONDS,
	HEARTS,
	SPADES,
	NUM_SUITS,	// number of suits
	BACK	// card back
} Suit;

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
	struct cardNode *next;	// ptr to the next card node in the list
} CardNode;


/**
 *	Pile is a linked list of card nodes
 */
typedef struct {
	int numCards;
	int numDecks;
	CardNode *pileTop, *pileBottom;	// ptrs to the pile top and bottom card nodes
} Pile;


Pile createPile();
void refillPile(Pile *cardPile);
CardNode *insertCardOnTop (Pile *pile, Card card);
CardNode *insertCardOnBottom (Pile *pile, Card card);
CardNode *dealCard(Pile *cardPile);
CardNode *removeCardFromTop(Pile *pile);
void listPile(Pile *pile);
bool pileIsEmpty(Pile *pile);

#endif /* end include guard */
