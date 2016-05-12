/**
 * Game mechanics
 */

#ifndef GAME_MECHANICS_H
#define	GAME_MECHANICS_H

#include "config.h"
#include "players.h"
#include "cards.h"

typedef enum {PLAYER, HOUSE} ActionSubject;

typedef enum {aHIT, aDOUBLE, aSURRENDER, aSTAND} eaAction;

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

void actionHit(GameTable *, Pile *, ActionSubject);

void actionStand(GameTable *);

void actionNewGame(GameTable *table, Pile *cardPile);

void actionDouble(GameTable *, Pile *);

void actionSurrender(GameTable *);

void actionBet(GameTable *);

void modifyBet(GameTable *);

void executeEAAction(GameTable *, Pile *pile, eaAction action);

#endif /* end include guard */
