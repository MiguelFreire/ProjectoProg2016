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
#endif /* end include guard */
