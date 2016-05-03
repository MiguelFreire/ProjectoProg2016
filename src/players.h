/**
 * @file
 *
 * Player and House related structures and functions to manipulate these
 * structures
 */

#ifndef PLAYERS_H
#define	PLAYERS_H

#include "config.h"
#include "cards.h"

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
 * Player stats
 */
typedef struct {
	int won;
	int lost;
	int tied;
} playerStats;

/**
 * PLayer payload structure
 */
typedef struct {
	playerType type;
	char *name[MAX_NAME_SIZE + 1];
	int money, bet;
	playerState state;
	int numCards, handValue;
	cardNode *hand;
	playerStats stats;
} Player;

/**
 * Player list node structure
 */
typedef struct playerNode {
	Player player;
	struct playerNode *next;
} playerNode;

/**
 * The player list structure (points to linked list)
 */
typedef struct {
	playerNode *head;
	int totalPlayers, playersInGame;
} playerList;



/**
 * House possible states: STANDARD(no special state), has a BLACKJACK or is
 * BUSTED
 */
typedef enum {HOUSE_STANDARD, HOUSE_BLACKJACK, HOUSE_BUSTED} houseState;

/**
 * House structure (points to linked list)
 */
typedef struct {
	houseState state;
	int numCards;
	cardNode *hand;
} House;

#endif /* end include guard */