////////////////////////////////////////////////////////////////////////////////
//									CARDS.H									  //
//																			  //
// Card structures and functions to manipulate the card pile				  //
////////////////////////////////////////////////////////////////////////////////

/**
 * CARD FUNCITIONS
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "config.h"
#include "util.h"
#include "errorHandling.h"

#include "cards.h"

int cardValues[CARD_RANKS] = {2,3,4,5,6,7,8,9,10,10,10,10,11}; // card values
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
 * @param      pile  pointer to the card pile
 */
void refillPile(Pile *pile){
	Card tmpCard;
	logPlay("\b", "Refilled pile");
	for (int deck = 0; deck < pile->numDecks; deck++){ // decks

		for (int suit = 0; suit < NUM_SUITS; suit++){ // suits
			tmpCard.suit = suit;

			for (int rank = 1; rank <= CARD_RANKS; rank++){ // ranks
				tmpCard.rank = rank;
				tmpCard.value = cardValues[rank-1];

				pile->pileBottom = insertCardOnBottom(pile, tmpCard);
			}
		}
	}

}

/**
 * @brief      Deals a random card from the pile
 *
 * @param      pile  the card pile from where to take the card
 *
 * @return     ptr to the dealt card node
 *
 * Takes a random card from the pile and returns a pointer to that node
 */
CardNode *dealCard(Pile *pile){
	CardNode *cur, *prev;
	int curIndex = 0;
	int cardIndex = rand()%(pile->numCards);

	cur = pile->pileTop;

	// advance till required index is found
	while (curIndex < cardIndex){
		prev = cur;
		cur = cur->next;

		curIndex ++;
	}

	if (cur == pile->pileTop){ // wanted card is the top one
		pile->pileTop = cur->next;
	} else { // wanted card is interior
		prev->next = cur->next;
	}

	pile->numCards -= 1;

	// reffil if needed
	if (pileIsEmpty(pile)) refillPile(pile);

	return cur;
}


/**
 * @brief      Inserts a card on the top of the pile
 *
 * @param      pile  ptr to the card pile
 * @param[in]  card  card contents
 *
 * @return     pointer to the new pile top
 */
CardNode *insertCardOnTop (Pile *pile, Card card){
	CardNode *newNode = (CardNode *) malloc(sizeof(CardNode));
	if(newNode == NULL) fireNotEnoughMemoryError("CardNode");
	newNode->card = card;
	newNode->next = pile->pileTop;
	if (pileIsEmpty(pile)){
		pile->pileBottom = newNode;
	}
	pile->numCards ++;
	return newNode;
}


/**
 * @brief      Inserts a card on the bottom of the pile
 *
 * @param      pile  ptr to the card pile
 * @param[in]  card  card contents
 *
 * @return     pointer to the new pile bottom
 */
CardNode *insertCardOnBottom (Pile *pile, Card card){
	CardNode *newNode = (CardNode *) malloc(sizeof(CardNode));
	if(newNode == NULL) fireNotEnoughMemoryError("CardNode");
	newNode->card = card;
	newNode->next = NULL;

	if (pileIsEmpty(pile)){
		pile->pileTop = newNode;
	}else{
		pile->pileBottom->next = newNode;
	}

	pile->numCards ++;
	return newNode;
}


/**
 * @brief      Removes the topmost card of the pile
 *
 * @param      top   ptr to the top of the pile
 *
 * @return     ptr to the new top
 */
CardNode *removeCardFromTop(Pile *pile){
	CardNode *tmp, *top;
	tmp = pile->pileTop;
	top = pile->pileTop->next;
	free(tmp);
	pile->numCards --;
	return top;
}


/**
 * @brief      Lists all pile contents
 removeCardFromTop(cardPile);*
 * @param[in]  pile  the pile to list
 */
void listPile(Pile *pile){
	CardNode *cur = pile->pileTop;
	while (cur != NULL){
		printf("This is a[%d] [%d]\n", cur->card.suit, cur->card.rank);
		cur = cur->next;
	}

	return;
}

/**
 * @brief      Checks if a pile is empty
 *
 * @param      pile  the pile to check
 *
 * @return     true if the pile is empty, false otherwise
 */
bool pileIsEmpty(Pile *pile){
	return (pile->pileTop == NULL || pile->pileBottom == NULL
		|| pile->numCards == 0);
}
