/**
 * @brief      Creates an empty player list
 *
 * @return     empty player list
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "players.h"

// PLayer functions

/**
 * @brief      Creates an empty player list structure and returns it
 *
 * @return     empty player list
 */
PlayerList createPlayerList(){
	PlayerList tmp = {0};
	return tmp;
}

/**
 * @brief      Creates a player node for player list
 *
 * @param      list        ptr to the player list
 * @param[in]  playerData  content to fill the player with
 *
 * @return     ptr to the newly created node (new tail)
 */
PlayerNode *createPlayer(PlayerList *list, Player playerData){
	PlayerNode *newPlayer = NULL;
	newPlayer = (PlayerNode *) malloc(sizeof(PlayerNode));

	newPlayer->player = playerData;
	newPlayer->next = NULL;

	if(!playerListIsEmpty(list))
		list->tail->next = newPlayer;
	else
		list->head = newPlayer;

	list->totalPlayers++;
	
	return newPlayer;
}

/**
 * @brief      Removes a player from the list (head)
 *
 * @param      list  ptr to the player list
 *
 * @return     ptr to the new list head
 */
PlayerNode *removePlayer(PlayerList *list){
	PlayerNode *tmp = NULL;
	CardNode *tmpCard = NULL;

	tmp = list->head;
	list->head = list->head->next;

	// remove cards from hand
	tmpCard = tmp->player.hand; 
	while (tmpCard != NULL){
		tmpCard = popHand(tmpCard, NULL, &tmp->player.numCards);
	}

	free(tmp);
	list->totalPlayers -= 1;
	return list->head;
}

/**
 * @brief      Lists all the player in the player list
 *
 * @param[in]  playerList  ptr to the player list to list
 */
void listPlayers (PlayerList *playerList){
	PlayerNode *cur = NULL;
	cur = playerList->head;
	for (int i = 0; i < playerList->totalPlayers; i++){
		printf("================\n");
		printf("%s\n", cur->player.name);
		printf("Type: %d\n", cur->player.type);
		printf("Money: %d\n", cur->player.money);
		printf("Bet: %d\n", cur->player.bet);
		cur = cur->next;
	}
	printf("=================\n");
}

/**
 * @brief      Checks if the player list is empty
 *
 * @param      head  ptr to the head (or tail) of the list
 *
 * @return     true if the list is empty, false otherwise
 */
bool playerListIsEmpty(PlayerList *list){
	return (list->head == NULL);
}


// House fucntions

/**
 * @brief      Creates an empty house structure and returns it
 *
 * @return     empty house structure
 */
House createHouse(){
	House house = {0};
	return house;
}


// hand functions

/**
 * @brief      Inserts a card (node) in the top of the hand
 *
 * @param      hand     ptr to the hand top
 * @param      newCard  ptr to the card node to insert to the hand
 *
 * @return     ptr to the new hand top
 */
CardNode *pushToHand(CardNode *hand, CardNode *newCard, int *numCards){
	if (!handIsEmpty(hand))
		newCard->next = hand;

	(*numCards)++;
	return newCard;
}

/**
 * @brief      Removes a card (node) from the top of the hand
 *
 * @param      hand         ptr to the hand top
 * @param      cardContent  ptr to the content of the removed node
 *
 * @return     ptr the new top
 */
CardNode *popHand(CardNode *hand, Card *cardContent, int *numCards){
	CardNode *tmp = hand;
	if ( cardContent != NULL){
		*cardContent = hand->card;
	}
	hand = hand->next;
	free(tmp);

	(*numCards)--;
	return hand;
}

/**
 * @brief      Retrieves the content of a card from the hand
 *
 * @param      hand        ptr to the hand top
 * @param[in]  cardNumber  number of the card to retrieve (counting from the
 *                         top)
 *
 * @return     content of the card retrieved
 */
Card peekHand(CardNode *hand, int cardNumber){

	for (int i = 1; i < cardNumber; i++){
		hand = hand->next;
	}

	return hand->card;
}

/**
 * @brief      Checks if the hand is empty
 *
 * @param      hand  ptr to the hand top
 *
 * @return     true if the hand is empty, false otherwise
 */

bool handIsEmpty(CardNode *hand){
	return hand == NULL;
}

