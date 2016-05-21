////////////////////////////////////////////////////////////////////////////////
//								PLAYERS.C									  //
//																			  //
// Player and House related structure and functions to manipulate these	  	  //
// structures																  //
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "util.h"

#include "cards.h"
#include "players.h"


////////////////////////////////////////////////////////////////////////////////
//								Players Funtions							  //
////////////////////////////////////////////////////////////////////////////////

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
	PlayerNode *tmp = NULL, *newHead = NULL;

	tmp = list->head;
	newHead = list->head->next;

	// remove cards from hand
	while (!handIsEmpty(tmp->player.hand)){
		tmp->player.hand = popHand(tmp->player.hand, NULL, &tmp->player.numCards);
	}

	free(tmp);
	list->totalPlayers -= 1;
	return newHead;
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



/**
 * @brief      Updates player money
 *
 * @param      player  ptr to the player to update money
 * @param[in]  amount  the amount of money to increase (negative to decrease)
 */
void updateMoney(Player *player, int amount){
	player->money += amount;
	player->stats.houseGains -= amount;
}



////////////////////////////////////////////////////////////////////////////////
//								House Funtions								  //
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief      Creates an empty house structure and returns it
 *
 * @return     empty house structure
 */
House createHouse(){
	House house = {0};
	return house;
}



////////////////////////////////////////////////////////////////////////////////
//									Hand Funtions							  //
////////////////////////////////////////////////////////////////////////////////

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
	else
		newCard->next = NULL;

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
 * @brief      Calculates the player's hand value and updates state acording
 *             to the hand value
 *
 * @param      player  ptr to the player
 *
 * @return     the player's hand value
 */
int updatePlayerHandValue(Player *player) {
	CardNode *curr = player->hand;
	int handValue = getHandValue(curr, NULL);

	if(handValue == 21 && player->numCards == 2){
		logPlay(player->name, "has a blackjack!");
		player->state = BLACKJACK;
		return handValue;

	} else if(handValue > 21) {
		logPlay(player->name, "got busted!");
		player->state = BUSTED;
		return handValue;

	} else {
		return handValue;
	}
}


/**
 * @brief      Calculates the house's hand value and updates its state
 *             acording to the hand value
 *
 * @param      house  ptr to the house
 *
 * @return     the house hand value
 */
int updateHouseHandValue(House *house) {
	CardNode *curr = house->hand;
	int handValue = getHandValue(curr, NULL);

	if(handValue == 21 && house->numCards == 2) {
		house->state = HOUSE_BLACKJACK;
		return handValue;

	} else if(handValue > 21) {
		house->state = HOUSE_BUSTED;
		return handValue;

	} else {
		return handValue;
	}
}


/**
 * @brief      Get the hand value
 *
 * @param      hand     ptr to the hand to count
 * @param[out] numAces  ptr to the number of aces in hand
 *
 * @return     The hand value.
 */
int getHandValue(CardNode *hand, int *numAces) {
	CardNode *curr = hand;
	int handValue = 0, aces = 0;
	Card card = {0};
	while(curr != NULL){
		card = curr->card;

		handValue += card.value;
		if (card.rank == 13) aces ++;

		curr = curr->next;
	}

	for (int i = 0; i < aces; i++){
		if (handValue > 21) handValue -= 10; // change ace's value to 1
	}

	if (numAces != NULL) *numAces = aces;

	return handValue;
}


/**
 * @brief      Counts the number of aces in the hand
 *
 * @param      hand  ptr to the hand
 *
 * @return     num of aces in hand
 */
int hasAces(CardNode *hand) {
	int numAces = 0;
	CardNode *curr = hand;
	while (curr != NULL) {
		if(curr->card.rank == 13) numAces++;
		curr = curr->next;
	}

	return numAces;
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
