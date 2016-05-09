/**
 * @brief      Creates an empty player list
 *
 * @return     empty player list
 */
#include <stdio.h>
#include <stdlib.h>

#include "players.h"


PlayerList createPlayerList(){
	PlayerList tmp = {0};
	return tmp;
}

PlayerNode *createPlayer(PlayerNode *tail, Player playerData){
	PlayerNode *newPlayer = NULL;
	newPlayer = (PlayerNode *) malloc(sizeof(PlayerNode));

	newPlayer->player = playerData;
	newPlayer->next = NULL;

	if(tail != NULL)
		tail->next = newPlayer;


	return newPlayer;
}

void listPlayers (PlayerList playerList){
	PlayerNode *cur = NULL;
	cur = playerList.head;
	for (int i = 0; i < playerList.totalPlayers; i++){
		printf("================\n");
		printf("%s\n", cur->player.name);
		printf("Type: %d\n", cur->player.type);
		printf("Money: %d\n", cur->player.money);
		printf("Bet: %d\n", cur->player.bet);
		cur = cur->next;
	}
	printf("=================\n");
}

CardNode *pushToHand(CardNode *hand, CardNode *newCard){
	if (hand != NULL)
		newCard->next = hand;

	return newCard;
}

Card peekHand(CardNode *hand, int cardNumber){

	for (int i = 1; i < cardNumber; i++){
		hand = hand->next;
	}

	return hand->card;
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
