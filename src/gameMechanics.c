////////////////////////////////////////////////////////////////////////////////
//								GAMEMECHANICS.C								  //
//																			  //
// Table structure and funtions related to table. Game actions funtions for   //
// players(EA and HU), house and bet colecting								  //
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "config.h"
#include "players.h"
#include "cards.h"
#include "util.h"
#include "main.h"
#include "gameMechanics.h"

//////////////////////////////////////////////////////////////////////////////
//							Game Table Funtions								//
//////////////////////////////////////////////////////////////////////////////

/**
 * @brief      Creates a game table struct
 *
 * @return     the created game table
 */
GameTable createGameTable(){
	GameTable tmp = {0};
	return tmp;
}

/**
 * @brief      Tests if a table slot is empty
 *
 * @param      slot  the slot to test
 *
 * @return     true if the slop is empty, false otherwise
 */
bool slotIsEmpty(PlayerNode *slot){
	return (slot == NULL);
}


//////////////////////////////////////////////////////////////////////////////
//								Action Funtions								//
//////////////////////////////////////////////////////////////////////////////

/**
 * @brief      Player action Hit
 *
 * @param      table     ptr to the table structure
 * @param      cardPile  ptr to the card pile structure
 *
 * @return     new game phase
 */
int actionHit(GameTable *table, Pile *cardPile) {
	Player *player = &(table->slots[table->currentPlayer]->player);
	logPlay(player->name, "Hit");
	player->state = HIT;
	// give a card
	player->hand = pushToHand(player->hand, dealCard(cardPile), &player->numCards);
	player->handValue = updatePlayerHandValue(player);
 
	// check if the player may hit again and stand if he doesn't
	if (player->state == BUSTED
		|| player->state == BLACKJACK
		|| player->handValue == 21)
	{
		return (actionStand(table));
	}

	// player may hit again
	// check current player type
	if (table->slots[table->currentPlayer]->player.type == CPU){
		return EA_PLAYING;
	}
	return PLAYERS_PLAYING;

}


/**
 * @brief      Player action stand
 *
 * @param      table  ptr to the game table structure
 *
 * @return     new game phase
 *
 *             altough actionStand() is a player action it is used by newGame to
 *             choose the first player to play
 */
int actionStand(GameTable *table) {
	// check if current player is not -1 because of newGame();
	if (table->currentPlayer >= 0 && table->currentPlayer < TABLE_SLOTS) {
		logPlay(table->slots[table->currentPlayer]->player.name, "stood!");
	}

	// choose next player or let house play if there is none
	do {
		table->currentPlayer++;
		if (table->currentPlayer >= TABLE_SLOTS) {
			return HOUSE_TURN;
		}
	} while (slotIsEmpty(table->slots[table->currentPlayer]) ||
		table->slots[table->currentPlayer]->player.state != STANDARD); // next player has a BLACKJACK

	// check for next player type
	if (table->slots[table->currentPlayer]->player.type == CPU){
		return EA_PLAYING;
	}
	return PLAYERS_PLAYING;

}


/**
 * @brief      Starts a new game
 *
 * @param      table     ptr to the table structure
 * @param      cardPile  ptr to the card pile structure
 *
 * @return     new game phase
 */
int actionNewGame(GameTable *table, Pile *cardPile) {
	Player *curPlayer = NULL;

	// remove broke players from the table
	for (int i = 0; i < TABLE_SLOTS; i++){
		if (!slotIsEmpty(table->slots[i]) && table->slots[i]->player.state == BROKE){
			table->slots[i] = NULL;
		}
	}

	// check if there are players to play
	bool thereArePlayers = false;
	for (int i = 0; i < TABLE_SLOTS; i++){
		if (!slotIsEmpty(table->slots[i])){
			thereArePlayers = true;
			break;
		}
	}
	if (!thereArePlayers){
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "New Game",
	"Can't start a new game. There are players to play," 
	" please add some players to start a new game", NULL);
		return WAITING_FOR_NEW_GAME;
	}

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

			updateMoney(curPlayer, -curPlayer->bet);
			curPlayer->state = STANDARD;
			curPlayer->betMultiplier = 1;
			curPlayer->handValue = updatePlayerHandValue(curPlayer);

		}
	}
	table->house->handValue = getHandValue(table->house->hand, NULL);
	table->house->state = HOUSE_WAITING;

	// choose first player to play
	table->currentPlayer = -1;
	int newPhase = actionStand(table);

	logPlay("\b","A New game has started!");
	return newPhase;
}


/**
 * @brief      Player action double
 *
 * @param      table     ptr to the game table structure
 * @param      cardPile  ptr to the card pile structure
 * @param[in]  action    alternative EA action
 *
 * @return     new game phase
 *
 *             Performs double validity check and in case the player is an EA
 *             takes action based on the parameter "action"
 */
int actionDouble(GameTable *table, Pile *cardPile, EAAction action) {
	Player *player = &(table->slots[table->currentPlayer]->player);
	int newPhase;

	// check if double is valid and take action acording to player type
	if(player->type == HUMAN && (player->state == HIT || player->money < player->bet)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Double",
		"Double not allowed. The player has already hit or does not have enough money", NULL);

		logPlay(player->name, "doubled!");

		return PLAYERS_PLAYING;

	} else if(player->type == CPU && (player->state == HIT || player->money < player->bet)) {
		// chosse the alternative EA action
		if(action == aHIT) { // Dh
			newPhase = actionHit(table, cardPile);
			return newPhase;
		} else if(action == aSTAND) { // Ds
			newPhase = actionStand(table);
			return newPhase;
		}
	}

	// handle money and state
	updateMoney(player, -player->bet);
	player->betMultiplier = DOUBLE_MULTIPLIER;
	player->state = DOUBLED;

	// mandatory hit and stand
	newPhase = actionHit(table, cardPile);
	// hit only stands when the player is busted or has 21 points 
	// so stand if he doesn't
	if (player->state != BUSTED && player->handValue != 21){ 
		newPhase = actionStand(table);
	}

	return newPhase;
}

/**
 * @brief      Player action surrender
 *
 * @param      table  ptr to the game table structure
 *
 * @return     new game phase
 */
int actionSurrender(GameTable *table) {
	Player *player = &(table->slots[table->currentPlayer]->player);

	// check if surrender is valid
	if(player->state == HIT && player->type == HUMAN)
		return PLAYERS_PLAYING;
	else if(player->state == HIT && player->type == CPU) {
		return actionStand(table);
	}

	// handle money and state
	updateMoney(player, player->bet/2);
	player->state = SURRENDERED;

	logPlay(player->name, "surrendered!");

	return actionStand(table);
}

/**
 * @brief      Changes a player's bet
 *
 * @param      table  ptr to the game table structure
 */
void actionBet(GameTable *table) {
	Player *player = NULL;

	// get player name
	char playerName[MAX_NAME_SIZE+1];

	printf("What's the name of the player?\n");
	printf("(Write CANCEL to quit)\n");

	fgets(playerName, MAX_NAME_SIZE, stdin);
	playerName[strlen(playerName)-1] = '\0';

	// check for abort
	if(strcmp("CANCEL",playerName) == 0) {
		printf("You can return to game window\n");
		return;
	}
	// search for player
	for(int i = 0; i < TABLE_SLOTS; i++) {
		if(!slotIsEmpty(table->slots[i]) && strcmp(table->slots[i]->player.name, playerName) == 0) {
			player = &(table->slots[i]->player);
			break;
		}
	}

	if(player == NULL) {
		printf("Player not found\n");
		actionBet(table);
		return;
	}

	// check if the player is too broke
	if (player->money < MIN_BET){
		printf("Player has less money than the minimum bet (%d)."
			"Can't change the bet\n", MIN_BET);
		printf("You can return to game window\n");
		return;
	}

	// get new bet for player
	char buffer[MAX_BUFFER_SIZE];
	int newBet = 0;
	bool error = false;
	do {
		if(error)
			printf("Player's bet value must be between: %d and %d\n",
			MIN_BET, (int)(MAX_BET_FACTOR*player->money));
		printf("New bet?\n");
		fgets(buffer, MAX_BUFFER_SIZE, stdin);
		sscanf(buffer, "%d", &newBet);
    } while(!(isBetween(newBet, MIN_BET, (int)(MAX_BET_FACTOR*player->money)))
    	&& (error = true) ); // set error for the second time the loop runs

    player->bet = newBet;
    printf("Bet set\n");
    printf("You can return to game window\n");

    // check if player was and is no longer broke
    if (player->state == BROKE && player->money >= player->bet)
		player->state = STANDARD;

    return;
}


/**
 * @brief      Adds a player to the table
 *
 * @param[in]  slotClicked  the number of slot clicked [0-3]
 * @param      playerList   ptr tot he player list structure
 * @param      table        ptr to the game table structure
 *
 * @return     new game phase
 */
int actionAddPlayer(int slotClicked, PlayerList *playerList, GameTable *table){
	Player newPlayer = {0};

	char buffer_money[MAX_BUFFER_SIZE] = {0};
	char buffer_bet[MAX_BUFFER_SIZE] = {0};

	clearTerminal();
	printf("Adding player at slot %d\n", slotClicked + 1);

	// ask for new player's name
	char playerName[MAX_NAME_SIZE+1];

	printf("What's the name of the player?\n");
	printf("(Write CANCEL to quit)\n");

	fgets(playerName, MAX_NAME_SIZE, stdin);
	playerName[strlen(playerName)-1] = '\0';

	if(strcmp("CANCEL",playerName) == 0) {
		printf("You can return to game window\n");
		return WAITING_FOR_NEW_GAME;
	}

	// assign name to player
	strcpy(newPlayer.name, playerName);

	// ask for new player's type
	char playerType[MAX_PLAYER_TYPE_SIZE + 1];
	bool error1 = false;

	printf("Player's type? HU or EA?\n");
	do {
		if(error1)
			printf("Player's type must be HU or EA\n");
		fgets(playerType, MAX_PLAYER_TYPE_SIZE + 1, stdin);
		playerType[strlen(playerType)] = '\0';

	// check input was one the possible types
    } while((((strcmp(playerType, "HU") != 0) && (strcmp(playerType, "EA") != 0))
		|| (!(strcmp(playerType, "HU") != 0) && !(strcmp(playerType, "EA") != 0)))
    	&& (error1 = true) ); // set error for the second time the loop runs

    // assign type to player
	if(strcmp(playerType, "HU") == 0){
		newPlayer.type = HUMAN;
	} else if (strcmp(playerType, "EA") == 0){
		newPlayer.type = CPU;
	}


	// ask for new player's money
	int playerMoney = 0;

	bool error2 = false;
	do {
		if(error2)
			printf("Player's money must be between: %d and %d\n",
		MIN_SEED_MONEY, MAX_SEED_MONEY );
		printf("Introduce the initial money for this player\n");
		fgets(buffer_money, MAX_BUFFER_SIZE, stdin);
		sscanf(buffer_money, "%d", &playerMoney);

	} while(!(isBetween(playerMoney, MIN_SEED_MONEY, MAX_SEED_MONEY))
		&& (error2 = true));

	// assign money to new player
	newPlayer.money = playerMoney;

	// ask for new player's bet
	int playerBet = 0;
	bool error3 = false;
	do {
		if(error3)
			printf("Player's bet must be between: %d and %.0f\n",
		MIN_BET, MAX_BET_FACTOR*playerMoney );
		printf("Introduce the initial bet for this player\n");
		fgets(buffer_bet, MAX_BUFFER_SIZE, stdin);
		sscanf(buffer_bet, "%d", &playerBet);

	} while(!(isBetween(playerBet, MIN_BET, MAX_BET_FACTOR*playerMoney))
		&& (error3 = true));

	// assign bet to new player
	newPlayer.bet = playerBet;
	newPlayer.betMultiplier = 1;

	// add player to the player list and to the game table
	playerList->tail = createPlayer(playerList, newPlayer);
	table->slots[slotClicked] = playerList->tail;

	printf("New player added\n");
	printf("You can return to the game window\n");

	return WAITING_FOR_NEW_GAME;
}


//////////////////////////////////////////////////////////////////////////////
//							House Mechanics Funtions						//
//////////////////////////////////////////////////////////////////////////////

/**
 * @brief      House hit action
 *
 * @param      table     ptr to the house structure
 * @param      cardPile  ptr to the card pile structure
 *
 * @return     new game phase
 */
int houseHit(GameTable *table, Pile *cardPile) {
	House *house = table->house;
	logPlay("House", "hit!");
	// give house a card
	house->hand = pushToHand(house->hand, dealCard(cardPile), &house->numCards);
	house->handValue = updateHouseHandValue(house);

	// check house busted or has a blackjack
	if(house->state == HOUSE_BUSTED || house->state == HOUSE_BLACKJACK) return COLECTING_BETS;
	return HOUSE_TURN;
}


/**
 * @brief      Decides house actions
 *
 * @param      table     ptr to the game table structure
 * @param      house     ptr to the house structure
 * @param      cardPile  ptr to the card pile structure
 *
 * @return     new game phase
 */
int houseTurn(GameTable *table, House *house, Pile *cardPile){
	// house hits till it has more than 16 points
	if (house->handValue < 17){
		return houseHit(table, cardPile);
	}

	// stand otherwise
	logPlay("House","stood!");


	if (house->state != HOUSE_WAITING) house->state = HOUSE_COLECTING;

	// check for blackjack
	updateHouseHandValue(house);

	return COLECTING_BETS;
}



/**
 * @brief      Performs bet collection/paying
 *
 * @param      table  prt to he game table structure
 * @param      house  ptr to the house structure
 *
 * @return     new game phase
 */
int colectBets(GameTable *table, House *house){
	Player *player = NULL;

	// find the next player to colect the bet
	while (slotIsEmpty(table->slots[table->currentPlayer])) {

		table->currentPlayer++;

		// finish clecting if there are no players left
		if (table->currentPlayer >= TABLE_SLOTS){
			return WAITING_FOR_NEW_GAME;
		}
	}

	player = &table->slots[table->currentPlayer]->player;


	// test WIN, LOSS or TIE and colect or pay bets
	if ( // TIE
		(house->state == HOUSE_BLACKJACK && player->state == BLACKJACK) // both have blackjack
		|| ( house->handValue == player->handValue	// both have the same points ..
		&& player->state != BUSTED	// .. but player isn't busted ..
		&& house->state != HOUSE_BLACKJACK && player->state != BLACKJACK ) // .. and none has blackjack
	){

		// update state and stats
		player->state = TIED;
		player->stats.tied ++;

		// give bet back to player
		updateMoney(player, player->bet * player->betMultiplier);

	} else if ( // LOSS
	    player->state == BUSTED // player busted
	    || player->state == SURRENDERED // player surrendered
		|| house->state == HOUSE_BLACKJACK // house has a blackjack
		|| (player->handValue < house->handValue && house->state != HOUSE_BUSTED) // house has more points than the player
		){

		// update state and stats
		if (player->money < player->bet){ // player has not enough money
			player->state = BROKE;
			logPlay(player->name, "is broke!");
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

		// update state and stats
		if (player->state == BLACKJACK){
			player->state = BLACKJACK; // keep state
		} else {
			player->state = WON;
		}
		player->stats.won ++;

		// give the bet back plus winnings
		if (player->state == BLACKJACK){
			updateMoney(player, player->bet * (1 + BLACKJACK_MULTIPLIER));
		} else {
			updateMoney(player, player->bet * (2 * player->betMultiplier));
		}

	}

	// colect the next player or finish if there are no more players
	table->currentPlayer++;
	if (table->currentPlayer >= TABLE_SLOTS)
		return WAITING_FOR_NEW_GAME;

	return COLECTING_BETS;
}
