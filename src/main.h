/**
 * Program flow functions
 */

#ifndef FILEIO_H
#define	FILEIO_H

#include "gameMechanics.h"
#include "settings.h"

typedef enum {START, PLAYERS_PLAYING, HOUSE_TURN, POINTS_COUTING, WAINTING_FOR_NEW_GAME, GAME_OVER} GamePhase;

GamePhase initGame (GameTable *table, Settings *settings, Pile *pile);

#endif /* end include guard */
