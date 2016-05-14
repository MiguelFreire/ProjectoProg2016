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

	// take everyone's bet, reset their states and update their hand values
	for (int i = 0; i < TABLE_SLOTS; i++){
		if(!slotIsEmpty(table->slots[i])){
			curPlayer = &table->slots[i]->player;

			curPlayer->money -= curPlayer->bet;
			curPlayer->state = STANDARD;
			curPlayer->handValue = updatePlayerHandValue(curPlayer);

		}
	}
	table->house->handValue = getHandValue(table->house->hand, table->house->numCards);
	table->house->state = HOUSE_WAITING;
	// select first player
	table->currentPlayer = 0;
	while(slotIsEmpty(table->slots[table->currentPlayer]) 
		|| table->slots[table->currentPlayer]->player.state == BLACKJACK){
		table->currentPlayer ++;
	}

	return PLAYERS_PLAYING;
}

int actionDouble(GameTable *table, Pile *cardPile) {
	Player *player = &(table->slots[table->currentPlayer]->player);
	if(player->state == HIT) return PLAYERS_PLAYING;

	player->money -= player->bet;
	player->betMultiplier = DOUBLE_MULTIPLIER;

	int newPhase;
	newPhase = actionHit(table, cardPile, PLAYER);
	if(player->state != BUSTED)
		newPhase = actionStand(table);
	return newPhase;
}

int actionSurrender(GameTable *table) {
	Player *player = &(table->slots[table->currentPlayer]->player);
	if(player->state == HIT) return PLAYERS_PLAYING;

	player->state = SURRENDERED;

	return actionStand(table);
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
	Player *player = NULL;

	if (table->currentPlayer >= TABLE_SLOTS)
		return WAITING_FOR_NEW_GAME;

	// find the next player to colect the bet
	while (slotIsEmpty(table->slots[table->currentPlayer])) {

		table->currentPlayer++;

		if (table->currentPlayer >= TABLE_SLOTS){
			return WAITING_FOR_NEW_GAME;
		} 
	}

	player = &table->slots[table->currentPlayer]->player;


	// test WIN, LOSS or TIE and colect or pay bets
	if ( // TIE
		(house->state == HOUSE_BLACKJACK && player->state == BLACKJACK) // both have blackjack
		|| (house->handValue == player->handValue && // both have the same points
		house->state != HOUSE_BLACKJACK && player->state != BLACKJACK) // and none has blackjack
	){
		player->state = TIED;
		player->stats.tied ++;
		player->money += player->bet * (player->betMultiplier + (1 - (player->state == BLACKJACK) * BLACKJACK_MULTIPLIER ));

	} else if ( // LOSE
	    player->state == BUSTED // player busted
	    || player->state == SURRENDERED // player surrendered
		|| house->state == HOUSE_BLACKJACK // house has a blackjack
		|| (player->handValue < house->handValue && house->state != HOUSE_BUSTED) // house has more points than the player
		){
		if (player->money < player->bet){
			player->state = BROKE;
		} else if (player->state == SURRENDERED){
			player->state = SURRENDERED; // keep state
		} else if (player->state == BUSTED){
			player->state = BUSTED; // keep state
		} else {
			player->state = LOST;
		}
		
		player->stats.lost ++;

	} else if ( // WIN
		player->state == BLACKJACK // player has blackjack
		|| house->state == HOUSE_BUSTED // house busted
		|| player->handValue > house->handValue // player has more points than house
	){

		if (player->state == BLACKJACK){
			player->state = BLACKJACK;
		} else {
			player->state = WON;
		}
		player->stats.won ++;
		player->money += player->bet * 2 * (player->betMultiplier); // give taken bet plus winnings
	}

	table->currentPlayer++;
	return COLECTING_BETS;
}

void bust(Player *player){
	player->money -= player->bet * player->betMultiplier;
}

int actionAddPlayer(int slotClicked, PlayerList *playerList, GameTable *table){
	Player newPlayer = {0};
	char buffer[MAX_BUFFER_SIZE] = {0};
	// ask for new player's name
	printf("What's the name of the player?\n");
	printf("(Write CANCEL to quit)\n");

	char playerName[MAX_BUFFER_SIZE]; // TODO mudar o tamanho disto para o maximo do nome

	fgets(playerName, MAX_BUFFER_SIZE, stdin);
	playerName[strlen(playerName)-1] = '\0';

	strcpy(newPlayer.name, playerName);

	// ask for new player's type
	printf("What's the type of the player (HU/EA)\n");
	char playerType[MAX_PLAYER_TYPE_SIZE + 1];

	fgets(playerType, MAX_PLAYER_TYPE_SIZE + 2, stdin);
	playerType[strlen(playerType)-1] = '\0';

	if(strcmp(playerType, "HU") == 0){
		newPlayer.type = HUMAN;
	} else if (strcmp(playerType, "EA") == 0){
		newPlayer.type = CPU;
	}

	// ask for new player's money
	printf("Introduce the initial money for this player\n");
	int playerMoney;

	fgets(buffer, MAX_BUFFER_SIZE, stdin);
	buffer[strlen(buffer)-1] = '\0';
	printf("%s\n", buffer);

	sscanf(buffer, "%d", &playerMoney);

	newPlayer.money = playerMoney;

	// ask for new player's bet
	printf("Introduce the initial bet for this player\n");
	int playerBet;

	fgets(buffer, MAX_BUFFER_SIZE, stdin);
	buffer[strlen(buffer)-1] = '\0';

	sscanf(buffer, "%d", &playerBet);

	newPlayer.bet = playerBet;
	newPlayer.betMultiplier = 1;

	// add player to the player list
	playerList->tail = createPlayer(playerList, newPlayer);

	table->slots[slotClicked] = playerList->tail;

	listPlayers(playerList);

	printf("New player added\n");

	return WAITING_FOR_NEW_GAME;
}
