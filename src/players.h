////////////////////////////////////////////////////////////////////////////////
//								PLAYERS.H									  //
// Player and House related structure and functions to manipulate these	  	  //
// structures																  //
////////////////////////////////////////////////////////////////////////////////

#ifndef PLAYERS_H
#define	PLAYERS_H

#include "config.h"
#include "cards.h"

////////////////////////////////////////////////////////////////////////////////
//									Players									  //
////////////////////////////////////////////////////////////////////////////////

/**
 * Player possible types: Human or CPU (AI)
 */
typedef enum {HUMAN, CPU} PlayerType;

/**
 * Player possible states
 */
typedef enum {
	STANDARD,	// player is "normal"
	HIT,	// player has hit
	BLACKJACK,	// player has a blackjack
	BUSTED, // player has busted
	BROKE,	// player is broke
	SURRENDERED,	// player has surrendered
	DOUBLED,	// player has doubled
	TIED,	// player tied a game
	WON,	// player won a game
	LOST	// plyaer lost a game
} PlayerState;

/**
 * Player stats
 */
typedef struct {
	int won;	// games won
	int lost;	// games lost
	int tied;	// games tied
	int houseGains;	// money house won with this player
} PlayerStats;

/**
 * Player payload structure
 */
typedef struct {
	PlayerType type;
	char name[MAX_NAME_SIZE + 1];
	int money, bet;
	float betMultiplier;	// bet multiplier
	PlayerState state;
	int numCards, handValue;
	CardNode *hand;	// ptr to the player hand top card node
	PlayerStats stats;
} Player;

/**
 * Player list node structure
 */
typedef struct playerNode {
	Player player;
	struct playerNode *next; // ptr to the next player list node
} PlayerNode;

/**
 * The player list structure (points to linked list)
 */
typedef struct {
	PlayerNode *head, *tail;	// head and tail pointers to player node list
	int totalPlayers;
} PlayerList;


PlayerList createPlayerList();
PlayerNode *createPlayer(PlayerList *list, Player playerData);
PlayerNode *removePlayer(PlayerList *list);
void listPlayers (PlayerList *playerList);
bool playerListIsEmpty(PlayerList *list);

void updateMoney(Player *player, int amount);

////////////////////////////////////////////////////////////////////////////////
// 									House									  //
////////////////////////////////////////////////////////////////////////////////

/**
 * House possible states: STANDARD(no special state), has a BLACKJACK or is
 * BUSTED
 */
typedef enum {
	HOUSE_WAITING,	// house is waiting for players to finish playing
	HOUSE_PLAYING,	// house is playing
	HOUSE_COLECTING,	// house is colecting/paying bets
	HOUSE_BLACKJACK,	// house has a blackjack
	HOUSE_BUSTED	// house is busted
} HouseState;

/**
 * House structure (points to linked list)
 */
typedef struct {
 	HouseState state;
 	int numCards, handValue;
 	CardNode *hand;	// ptr to the house hand top card node
} House;


House createHouse();


////////////////////////////////////////////////////////////////////////////////
// 							Hand Functions									  //
////////////////////////////////////////////////////////////////////////////////

CardNode *pushToHand(CardNode *hand, CardNode *newCard, int *numCards);
CardNode *popHand(CardNode *hand, Card *cardContent, int *numCards);
Card peekHand(CardNode *hand, int cardNumber);
bool handIsEmpty(CardNode *hand);
int getHandValue(CardNode *hand, int *numAces);
int hasAces(CardNode *hand);

int updatePlayerHandValue(Player *player);
int updateHouseHandValue(House *house);
bool pileIsEmpty (Pile *pile);

#endif /* end include guard */
