/**
 * Game mechanics
 */

#ifndef GAME_MECHANICS_H
#define	GAME_MECHANICS_H

#include "config.h"
#include "players.h"
#include "cards.h"


typedef enum {aHIT, aDOUBLES, aDOUBLEH, aSURRENDER, aSTAND} EAAction;

typedef struct {
	int x;	// x position
	int y;	// y position
	int w;	// width
	int h;	// height
} SlotDim;

typedef struct {
	int currentPlayer;
	int numPlayersInGame;
	PlayerNode *slots[TABLE_SLOTS];
	SlotDim slotDim[TABLE_SLOTS];
	House *house;
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

void executeEAAction(GameTable *, Pile *pile, EAAction action);

#endif /* end include guard */
