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
	playerNode *players[TABLE_SLOTS];
	House house;
	Pile pile;
} gameTable;

#endif /* end include guard */