/**
 * Card related data structures and functions to handle this structures
 */

#ifndef CARDS_H
#define	CARDS_H



/**
 * Enum type of suit
 */
typedef enum {club=1, diamonds, hearts, spades} Suit;

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
} cardNode;


/**
 *	Pile is a linked list
 */
typedef struct {
	int numCards;
	cardNode *pile;
} Pile;


/*****************
 * Stack FUNCTION
 ****************/
/*cardNode* initHand() {
  return NULL;
}

cardNode* pushCard(cardNode* top) {
  
}

cardNode* popCard() {

}

*/
#endif /* end include guard */
