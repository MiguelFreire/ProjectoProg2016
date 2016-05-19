/**
 * Program flow functions
 */

#ifndef FILEIO_H
#define	FILEIO_H

#include <SDL2/SDL.h>

#include "gameMechanics.h"
#include "settings.h"

typedef enum {
	START,
	PLAYERS_PLAYING,
	EA_PLAYING,
	HOUSE_TURN,
	COLECTING_BETS,
	WAITING_FOR_NEW_GAME,
	ADDING_PLAYER,
	GAME_OVER
} GamePhase;

GamePhase initGame (GameTable *table, PlayerList *playerList, Pile *pile,
	House *house, Settings *settings, char *argv1);

GamePhase handleKeyPress(SDL_Event *event, GameTable *table, Pile *pile, GamePhase phase, bool *quit, int *EASpeed, int *EADelay);

GamePhase handleMousePress(SDL_Event *event, GameTable *table, PlayerList *playerList, GamePhase phase);

void freeEverything(PlayerList *playerList, House *house, Pile *cardPile, Settings *settings, int **softMatrix, int **hardMatrix);

#endif /* end include guard */
