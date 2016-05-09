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
typedef enum {HUMAN, CPU} PlayerType;

/**
 * Player possible states: STANDARD(no special state), has a BLACKJACK, is
 * BUSTED or is BROKE
 */
typedef enum {STANDARD, HIT, BLACKJACK, BUSTED, BROKE, SURRENDERED} PlayerState;

/**
 * Player stats
 */
typedef struct {
	int won;
	int lost;
	int tied;
} PlayerStats;

/**
 * PLayer payload structure
 */
typedef struct {
	PlayerType type;
	char name[MAX_NAME_SIZE + 1];
	int money, bet, betMultiplier;
	PlayerState state;
	int numCards, handValue;
	CardNode *hand;
	PlayerStats stats;
} Player;

/**
 * Player list node structure
 */
typedef struct playerNode {
	Player player;
	struct playerNode *next;
} PlayerNode;

/**
 * The player list structure (points to linked list)
 */
typedef struct {
	PlayerNode *head, *tail;
	int totalPlayers;
} PlayerList;


PlayerList createPlayerList();

PlayerNode *createPlayer(PlayerNode *tail, Player playerData);
void listPlayers (PlayerList playerList);

CardNode *pushToHand(CardNode *hand, CardNode *newCard);

Card peekHand(CardNode *hand, int cardNumber);


/**
 * House possible states: STANDARD(no special state), has a BLACKJACK or is
 * BUSTED
 */
typedef enum {HOUSE_WAITING, HOUSE_STANDARD, HOUSE_BLACKJACK, HOUSE_BUSTED} HouseState;

/**
 * House structure (points to linked list)
 */
typedef struct {
	HouseState state;
	int numCards;
	CardNode *hand;
} House;

House createHouse();

#endif /* end include guard */
