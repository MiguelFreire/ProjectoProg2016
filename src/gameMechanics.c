#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "config.h"
#include "players.h"
#include "cards.h"
#include "util.h"
#include "main.h"
#include "gameMechanics.h"

GameTable createGameTable(){
	GameTable tmp = {0};
	return tmp;
}

bool slotIsEmpty(PlayerNode *slot){
	return (slot == NULL);
}

int actionHit(GameTable *table, Pile *cardPile, ActionSubject subject) {
	if(subject == PLAYER) {
		Player *player = &(table->slots[table->currentPlayer]->player);
		player->state = HIT;
		player->hand = pushToHand(player->hand, dealCard(cardPile), &player->numCards);
		player->handValue = updatePlayerHandValue(player);
		printf("player %d - %d\n", player->state, player->handValue);
		if (player->state == BUSTED){
			bust(player);
		}

		if(player->state == BUSTED || player->state == BLACKJACK) return (actionStand(table));
	} else if(subject == HOUSE) {
		House *house = table->house;
		house->hand = pushToHand(house->hand, dealCard(cardPile), &house->numCards);
		house->handValue = updateHouseHandValue(house);
		printf("house %d - %d\n", house->state, house->handValue);

		if(house->state == HOUSE_BUSTED || house->state == HOUSE_BLACKJACK) return COLECTING_BETS;
		return HOUSE_TURN;
	}
	return PLAYERS_PLAYING;
}

int actionStand(GameTable *table) {
	do {
		table->currentPlayer++;
		if (table->currentPlayer >= TABLE_SLOTS) return HOUSE_TURN;
	} while (slotIsEmpty(table->slots[table->currentPlayer]) ||
		table->slots[table->currentPlayer]->player.state != STANDARD); // next player has a BLACKJACK
	
	return PLAYERS_PLAYING;
}

int actionNewGame(GameTable *table, Pile *cardPile) {
	Player *curPlayer = NULL;

	// empty house's hand
	while (table->house->hand != NULL){
		table->house->hand = popHand(table->house->hand, NULL, &table->house->numCards);
	}

	// empty players hands
	for (int i = 0; i < TABLE_SLOTS; i++){
		if(!slotIsEmpty(table->slots[i])){
			curPlayer = &table->slots[i]->player;
			while (curPlayer->hand != NULL){
				curPlayer->hand = popHand(curPlayer->hand, NULL, &curPlayer->numCards);
			}
		}
	}
	

	// deal 2 cards to each player and house
	for (int i = 0; i < 2; i++){
		// hand a card to each player
		for (int j = 0; j < TABLE_SLOTS; j++){
			if(!slotIsEmpty(table->slots[j])){
				curPlayer = &table->slots[j]->player;
				curPlayer->hand = pushToHand(curPlayer->hand, dealCard(cardPile), &curPlayer->numCards);
			}
		}
	// hand a card to the house
	table->house->hand = pushToHand(table->house->hand , dealCard(cardPile), &table->house->numCards);
	}

	// take everyone's bet and reset their states
	for (int i = 0; i < TABLE_SLOTS; i++){
		if(!slotIsEmpty(table->slots[i])){
			curPlayer = &table->slots[i]->player;
			curPlayer->money -= curPlayer->bet;
			curPlayer->state = STANDARD;
		}
	}

	// reset current player
	table->currentPlayer = 0;

	return PLAYERS_PLAYING;
}

void actionDouble(GameTable *table, Pile *cardPile) {
	Player *player = &(table->slots[table->currentPlayer]->player);
	if(player->state == HIT) return;

	player->money -= player->bet;
	player->betMultiplier = DOUBLE_MULTIPLIER;

	actionHit(table, cardPile, PLAYER);
	actionStand(table);
}

void actionSurrender(GameTable *table) {
	Player *player = &(table->slots[table->currentPlayer]->player);
	if(player->state == HIT) return;

	player->state = SURRENDERED;

	actionStand(table);
}

void actionBet(GameTable *table) {
	printf("What's the name of the player?\n");
	printf("(Write CANCEL to quit)\n");

	char playerName[MAX_BUFFER_SIZE];

	fgets(playerName, MAX_BUFFER_SIZE, stdin);
	playerName[strlen(playerName)-1] = '\0';

	Player *player = NULL;

	if(strcmp("CANCEL",playerName) == 0) return;

	for(int i = 0; i < TABLE_SLOTS; i++) {
		if(!slotIsEmpty(table->slots[i]) && strcmp(table->slots[i]->player.name, playerName) == 0) {
			player = &(table->slots[i]->player);
		}
	}

	if(player == NULL) {
		printf("Player not found\n");
		actionBet(table);
	}

	char buffer[MAX_BUFFER_SIZE];
	int newBet = 0;
	bool error = false;
	do {
		if(error) printf("Player's bet value must be between: %d and %.2f\n", 2, 0.25*player->money);
		printf("New bet?\n");
		fgets(buffer, MAX_BUFFER_SIZE, stdin);
		sscanf(buffer, "%d", &newBet);
   } while(!(isBetween(newBet, 2, 0.25*player->money)) && (error = true));

   player->bet = newBet;
   printf("Bet set\n");

   return;
}

int houseTurn(GameTable *table, House *house, Pile *cardPile){
	if (house->handValue < 17){
		return actionHit(table, cardPile, HOUSE);
	}
	house->state = COLECTING_BETS;
	return COLECTING_BETS;
}

int colectBets(GameTable *table, House *house){
	table->currentPlayer = 0;
	Player *player = &table->slots[table->currentPlayer]->player;

	// find the next player to colect the bet
	// busted players had their bet colected already, so they are passed
	while (slotIsEmpty(table->slots[table->currentPlayer]) 
		|| player->state == WON || player->state == LOST 
		|| player->state == TIED || player->state == BUSTED) {

		table->currentPlayer++;
		if (table->currentPlayer >= TABLE_SLOTS) return WAITING_FOR_NEW_GAME;
		player = &table->slots[table->currentPlayer]->player;

	} 
	
	
	// test WIN, LOSS or TIE and colect or pay bets
	if ( // TIE
		(house->state == HOUSE_BLACKJACK && player->state == BLACKJACK) // both have blackjack
		|| (house->handValue == player->handValue && // both have the same points
		house->state != HOUSE_BLACKJACK && player->state != BLACKJACK) // and none has blackjack
	){ 
		player->state = TIED;
		player->money += player->bet * (player->betMultiplier + (1 - (player->state == BLACKJACK) * BLACKJACK_MULTIPLIER ));

	} else if ( // LOSE
		house->state == HOUSE_BLACKJACK // house has a blackjack
		|| (player->handValue < house->handValue && house->state != HOUSE_BUSTED) // house has more points than the player
		){
		player->state = LOST;

	} else if ( // WIN
		player->state == BLACKJACK // player has blackjack 
		|| house->state == HOUSE_BUSTED // house busted
		|| player->handValue > house->handValue // player has more points than house
	){ 
		player->state = WON;
		player->money += player->bet * (1 + player->betMultiplier); // give taken bet plus winnings
	}

	return COLECTING_BETS;
}

void bust(Player *player){
	player->money -= player->bet * player->betMultiplier;
	// take the cards from player hand
	while (player->hand != NULL){
		player->hand = popHand(player->hand, NULL, &player->numCards);
	}
}