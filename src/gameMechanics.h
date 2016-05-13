/**
 * Game mechanics
 */

#ifndef GAME_MECHANICS_H
#define	GAME_MECHANICS_H

#include "config.h"
#include "players.h"
#include "cards.h"


typedef enum {PLAYER, HOUSE} ActionSubject;

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

int actionHit(GameTable *, Pile *, ActionSubject);

int actionStand(GameTable *);

int actionNewGame(GameTable *table, Pile *cardPile);

void actionDouble(GameTable *, Pile *);

void actionSurrender(GameTable *);

void actionBet(GameTable *);

int houseTurn(GameTable *table, House *house, Pile *cardPile);

int colectBets(GameTable *table, House *house);

void bust(Player *player);

void modifyBet(GameTable *);

#endif /* end include guard */
