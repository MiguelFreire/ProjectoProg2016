////////////////////////////////////////////////////////////////////////////////
//								GAMEMECHANICS.H								  //
//																			  //
// Table structure and funtions related to table. Game actions funtions for   //
// players(EA and HU), house and bet colecting								  //
////////////////////////////////////////////////////////////////////////////////

#ifndef GAME_MECHANICS_H
#define	GAME_MECHANICS_H

#include "config.h"
#include "players.h"
#include "cards.h"

/**
 * EA matrixes possible values
 */
typedef enum {
	aHIT,	// EA Hit
	aDOUBLES,	// EA double (stand when not allowed)
	aDOUBLEH,	// EA double (hit when not allowed)
	aSURRENDER,	// EA surrender
	aSTAND	// EA stand
} EAAction;

/**
 * Table slots dimensions
 */
typedef struct {
	int x;	// x position
	int y;	// y position
	int w;	// width
	int h;	// height
} SlotDim;

/**
 * Game table structure
 */
typedef struct {
	int currentPlayer;
	int numPlayersInGame;
	PlayerNode *slots[TABLE_SLOTS];	// player areas
	SlotDim slotDim[TABLE_SLOTS];
	House *house;	// pointer to the house structure
} GameTable;


GameTable createGameTable();
bool slotIsEmpty(PlayerNode *slot);

int actionHit(GameTable *table, Pile *cardPile);
int actionStand(GameTable *);
int actionNewGame(GameTable *table, Pile *cardPile);
int actionDouble(GameTable *, Pile *, EAAction);
int actionSurrender(GameTable *);
void actionBet(GameTable *);
int actionAddPlayer(int slotClicked, PlayerList *playerList, GameTable *table);

int houseHit(GameTable *table, Pile *cardPile);
int houseTurn(GameTable *table, House *house, Pile *cardPile);
int colectBets(GameTable *table, House *house);

void modifyBet(GameTable *);

#endif /* end include guard */
