/*****************
 * HAND FUNCTIONS
 ****************/
// Hand* initHand() {
//   return NULL;
// }
//
// Hand* pushCard(Card* top) {
//   int tmp = top->numCards;
//   Card* new_card = (Card*) malloc(sizeof(Card));
//   if(new_card == NULL) {
//     exit("Memory is full");
//   }
//   new_card->owner = owner;
//   new_card->next = top;
//   return new_card;
// }

// bool isHandEmpty(Hand* top) {
//   return top == NULL ? true : false;
// }

/**
 * CARD FUNCITIONS
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "config.h"
#include "errorHandling.h"
#include "cards.h"

/**
 * @brief      Creates an empty card pile
 *
 * @return     empty card pile
 */
Pile createPile(){
	Pile tmp = {0};
	return tmp;
}

/**
 * @brief      (Re)fills the card pile with card decks
 *
 * @param      cardPile  pointer to the card pile
 * @pqaram[in]  numDecks  number of decks to fill the pile with
 */
void refillPile(Pile *cardPile, int numDecks){
	Card tmpCard;
	printf("Refiling pile\n");
	for (int deck = 0; deck < numDecks; deck++){

		for (int suit = 0; suit < NUM_SUITS; suit++){
			tmpCard.suit = suit;

			for (int rank = 1; rank <= CARD_RANKS; rank++){
				tmpCard.rank = rank;
				if (pileIsEmpty(*cardPile)){
					cardPile->pileBottom = insertCardOnBottom(cardPile->pileBottom, tmpCard);
					cardPile->pileTop = cardPile->pileBottom;
				} else {
					cardPile->pileBottom->next = insertCardOnBottom(cardPile->pileBottom, tmpCard);
					cardPile->pileBottom = cardPile->pileBottom->next;
				}
				
				cardPile->numCards++;
				
				printf("[%d] [%d] [%d] [%d]\n", cardPile->numCards, deck, suit, rank);
			}
		}
	}
	
}

void dealCard(CardNode **hand, Pile *cardPile){
	int cardIndex = rand()%(cardPile->numCards);

	printf("Card index dealt %d\n", cardIndex);
	int curIndex = 0;
	CardNode *cur, *prev;

	printf("nope\n");
	cur = cardPile->pileTop;
	printf("nope\n");
	while (curIndex < cardIndex){
		prev = cur;
		printf("nope2\n");
		cur = cur->next;
		printf("nope3\n");
	}
	printf("nope4\n");
	if (cur == cardPile->pileTop){
		cardPile->pileTop = cur->next;
	} else {
		printf("nope\n");
		prev->next = cur->next;
	}
	
	printf("nope\n");
	if (hand == NULL){
		cur->next = NULL;
		free (cur);
		cur = NULL;
		printf("nope\n");
	} else {
		cur->next = *hand;
		*hand = cur;
	}
	printf("nope\n");

	printf("Dealt a [%d] [%d]\n", (*hand)->card.suit, (*hand)->card.rank);

	return;
}


/**
 * @brief      Checks if a pile is empty
 *
 * @param[in]  pile  the pile to check
 *
 * @return     true if the pile is empty, false otherwise
 */
bool pileIsEmpty(Pile pile){
	if (pile.pileTop == NULL || pile.pileBottom == NULL)
		return true;
	else
		return false;
}


/**
 * @brief      Inserts a card on the top of the pile
 *
 * @param      top   pointer to the pile top node
 * @param[in]  card  card contents
 *
 * @return     pointer to the new node
 */
CardNode *insertCardOnTop (CardNode *top, Card card){
	CardNode * newNode = (CardNode *) malloc(sizeof(CardNode));
	if(newNode == NULL) fireNotEnoughMemoryError("CardNode");
	newNode->card = card;
	newNode->next = top;
	return newNode;
}

/**
 * @brief      Inserts a card on the bottom of the pile
 *
 * @param      bottom  pointer to the pile bottom node
 * @param[in]  card    card contents
 *
 * @return     pointer to the new node
 */
CardNode *insertCardOnBottom (CardNode *bottom, Card card){
	CardNode *newNode = (CardNode *) malloc(sizeof(CardNode));
	if(newNode == NULL) fireNotEnoughMemoryError("CardNode");
	newNode->card = card;
	newNode->next = NULL;
	return newNode;
}

void removeCard(CardNode **pileTop, int cardIndex){
	

}