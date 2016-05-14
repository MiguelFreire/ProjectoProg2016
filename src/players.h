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

// Players

/**
 * Player possible types: Human or CPU (AI)
 */
typedef enum {HUMAN, CPU} PlayerType;

/**
 * Player possible states: STANDARD(no special state), has a BLACKJACK, is
 * BUSTED or is BROKE
 */
typedef enum {
	STANDARD,
	HIT,
	BLACKJACK,
	BUSTED,
	BROKE,
	SURRENDERED,
	DOUBLED,
	TIED,
	WON,
	LOST
} PlayerState;

/**
 * Player stats
 */
typedef struct {
	int won;
	int lost;
	int tied;
	int houseGains;
} PlayerStats;

/**
 * PLayer payload structure
 */
typedef struct {
	PlayerType type;
	char name[MAX_NAME_SIZE + 1];
	int money, bet;
	float betMultiplier;
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
PlayerNode *createPlayer(PlayerList *list, Player playerData);
PlayerNode *removePlayer(PlayerList *list);
void listPlayers (PlayerList *playerList);
bool playerListIsEmpty(PlayerList *list);

void updateMoney(Player *player, int amount);

// House

/**
 * House possible states: STANDARD(no special state), has a BLACKJACK or is
 * BUSTED
 */
typedef enum {HOUSE_WAITING, HOUSE_COLECTING, HOUSE_BLACKJACK, HOUSE_BUSTED} HouseState;

/**
 * House structure (points to linked list)
 */
typedef struct {
 	HouseState state;
 	int numCards, handValue;
 	CardNode *hand;
} House;


House createHouse();



// hand functions
CardNode *pushToHand(CardNode *hand, CardNode *newCard, int *numCards);
CardNode *popHand(CardNode *hand, Card *cardContent, int *numCards);
Card peekHand(CardNode *hand, int cardNumber);
bool handIsEmpty(CardNode *hand);
int updatePlayerHandValue(Player *player);
int updateHouseHandValue(House *house);

#endif /* end include guard */
