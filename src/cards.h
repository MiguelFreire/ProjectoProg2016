/**
 * Card related data structures and functions to handle this structures
 */

#ifndef CARDS_H
#define	CARDS_H

/**
 * Enum type of suit
 */
enum Suit{club=1, diamonds, hearts, spades}

/**
 * Card is a ordinary struct
 */
typedef struct {
  int rank;
  Suit suit;
  int value;
} Card;

/**
 *	Deck is a linked list
 */
typedef struct {
  Card *stack;
  struct Deck *next;
} Deck;

/**
 * Hand is a stack
 */
typedef struct {
  Player owner;
  Hand   *next;
} Hand;

/*****************
 * Stack FUNCTION
 ****************/
Hand* initHand() {
  return NULL;
}

Hand* pushCard(Hand* top, Player owner) {

}

Hand* popCard() {

}


#endif /* end include guard */
