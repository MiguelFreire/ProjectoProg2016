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

/**
 * @brief      Deals a card from the pile
 *
 * @param      cardPile  the card pile from where to take the card
 *
 * @return     ptr to the removed card node
 * 
 * Takes a random card from the pile and returns a pointer to the node
 */
CardNode *dealCard(Pile *cardPile){
	CardNode *cur, *prev;
	int curIndex = 0;
	int cardIndex = rand()%(cardPile->numCards);
	printf("Card index dealt %d\n", cardIndex);

	cur = cardPile->pileTop;

	// advance till required index is found
	while (curIndex < cardIndex){
		prev = cur;
		cur = cur->next;

		curIndex ++;
	}

	if (cur == cardPile->pileTop){ // wanted card is the top one
		cardPile->pileTop = cur->next;
	} else { // wanted card is interior
		prev->next = cur->next;
	}

	printf("Dealt a [%d] [%d]\n", cur->card.suit, cur->card.rank);
	return cur;
}

/**
 * @brief      Lists all pile contents
 *
 * @param[in]  pile  the pile to list
 */
void listPile(Pile pile){
	CardNode *cur = pile.pileTop;
	while (cur != NULL){
		printf("This is a[%d] [%d]\n", cur->card.suit, cur->card.rank);
		cur = cur->next;
		
	}

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
	CardNode *newNode = (CardNode *) malloc(sizeof(CardNode));
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