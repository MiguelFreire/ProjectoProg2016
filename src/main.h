/**
 * Program flow functions
 */

#ifndef FILEIO_H
#define	FILEIO_H

#include <SDL2/SDL.h>

#include "gameMechanics.h"
#include "settings.h"

/**
 * Phase of the game, used for flow control
 */
typedef enum {
	START,	// program started
	PLAYERS_PLAYING,	// human players are playing
	EA_PLAYING,	// EA players are playing
	HOUSE_TURN,	// house is playing
	COLECTING_BETS, // house is colecting or paying bets
	WAITING_FOR_NEW_GAME,	// game is halted and waiting for a new game
	ADDING_PLAYER,	// a player is being added to the table
} GamePhase;

GamePhase initGame (GameTable *table, PlayerList *playerList, Pile *pile,
	House *house, Settings *settings, char *argv1);

GamePhase handleKeyPress(SDL_Event *event, GameTable *table, Pile *pile, 
	GamePhase phase, bool *quit, int *EASpeed, int *EADelay);

GamePhase handleMousePress(SDL_Event *event, GameTable *table, 
	PlayerList *playerList, GamePhase phase);

void freeEverything(PlayerList *playerList, House *house, Pile *cardPile, 
	Settings *settings, int **softMatrix, int **hardMatrix);

#endif /* end include guard */
