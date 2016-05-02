/**
 * @file
 *
 * Player and House related structures and functions to manipulate these
 * structures
 */

#ifndef PLAYERS_H
#define	PLAYERS_H

#include "config.h"

/**
 * Player possible types: Human or CPU (AI)
 */
typedef enum {HUMAN, CPU} playerType;

/**
 * Player possible states: STANDARD(no special state), has a BLACKJACK, is
 * BUSTED or is BROKE
 */
typedef enum {STANDARD, BLACKJACK, BUSTED, BROKE} playerState;

/**
 * The player list structure (points to linked list)
 */
typedef struct {
	Player *head;
	int totalPlayers, playersInGame;
} playerList;

/**
 * Player structure (linked list node and points to other linked list)
 */
typedef struct {
	playerType type;
	char *name[MAX_NAME_SIZE + 1];
	int money, bet;
	state state;
	int numCards, handValue;
	Card *hand;
	Player *nextPlayer;
} Player;


/**
 * House possible states: STANDARD(no special state), has a BLACKJACK or is
 * BUSTED
 */
typedef enum {STANDARD, BLACKJACK, BUSTED} houseState;

/**
 * House structure (points to linked list)
 */
typedef struct {
	houseState state;
	int numCards;
	Card *hand;
} House;

#endif /* end include guard */