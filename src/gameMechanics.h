/**
 * Game mechanics
 */

#ifndef GAME_MECHANICS_H
#define	GAME_MECHANICS_H

#include "config.h"
#include "players.h"
#include "cards.h"

typedef struct {
	int currentPlayer;
	PlayerNode *slots[TABLE_SLOTS];
} GameTable;

GameTable createGameTable();

void actionHit(GameTable *);

void actionStand(GameTable *);

void actionNewGame(GameTable *);

void actionDouble(GameTable *);

void actionSurrender(GameTable *);

void actionBet(GameTable *);

#endif /* end include guard */
