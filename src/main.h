/**
 * Program flow functions
 */

#ifndef FILEIO_H
#define	FILEIO_H

#include "gameMechanics.h"
#include "settings.h"

typedef enum {START, PLAYERS_PLAYING, HOUSE_TURN} GamePhase;

GamePhase initGame (GameTable *table, Settings *settings, Pile *pile);

#endif /* end include guard */
