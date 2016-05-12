/**
 * Program flow functions
 */

#ifndef FILEIO_H
#define	FILEIO_H

#include "gameMechanics.h"
#include "settings.h"

typedef enum {
	START, 
	PLAYERS_PLAYING, 
	HOUSE_TURN, 
	COLECTING_BETS, 
	WAITING_FOR_NEW_GAME,
	ADDING_PLAYER, 
	GAME_OVER
} GamePhase;

GamePhase initGame (GameTable *table, PlayerList *playerList, Pile *pile,
	House *house, Settings *settings, char *argv1);

void freeEverything(PlayerList *playerList, House *house, Pile *cardPile, Settings *settings);

#endif /* end include guard */
