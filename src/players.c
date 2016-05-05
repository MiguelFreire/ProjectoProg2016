/**
 * @brief      Creates an empty player list
 *
 * @return     empty player list
 */

#include "players.h"

PlayerList createPlayerList(){
	PlayerList tmp = {0};
	return tmp;
}

PlayerNode *createPlayer(PlayerNode *tail, Player playerData){
	Player *newPlayer = NULL;
	newPlayer = (PlayerNode *) malloc(sizeof(PlayerNode));
	
	newPlayer.player = playerData;
	tail->next = newPLayer;

	return newPlayer;
}

CardNode *pushToHand(CardNode *hand, CardNode *newCard){
	hand->next = hand;

	return newCard;
}