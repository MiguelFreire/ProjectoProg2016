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

int updatePlayerHandValue(Player *player, int numCards) {
	CardNode *next;
	int numAces = 0;
	int handValue = 0;
	for(int i = 0; i < numCards; i++){
		Card card = player->hand->card;

		if(card.rank == 13) numAces++;
		handValue += card.value;

		next = player->hand->next;
	}

	if(handValue == 21) {
		player->state = BLACKJACK;
		return handValue;
	} else if(handValue > 21) {
		player->state = BUSTED;
		return handValue;
	} else if(handValue > 21 && numAces > 0) {
		for(int k = 1; k <= numAces; k++) {
        	if(handValue <= 21) break;
        	handValue -= 10;
        }

		return handValue;
	}

	return handValue;

}

int updateHouseHandValue(House *house, int numCards) {
	CardNode *next;
	int numAces = 0;
	int handValue = 0;

	for(int i = 0; i < numCards; i++){
		Card card = house->hand->card;

		if(card.rank == 13) numAces++;
		handValue += card.value;

		next = house->hand->next;
	}

	if(handValue == 21) {
		house->state = HOUSE_BLACKJACK;
		return handValue;
	} else if(handValue > 21) {
		house->state = HOUSE_BUSTED;
		return handValue;
	} else if(handValue > 21 && numAces > 0) {
		for(int k = 1; k <= numAces; k++) {
        	if(handValue <= 21) break;
        	handValue -= 10;
        }

		return handValue;
	}

	return handValue;
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
