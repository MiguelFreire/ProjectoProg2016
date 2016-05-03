/**
 * Program flow functions
 */

#ifndef FILEIO_H
#define	FILEIO_H

#include "gameMechanics.h"

typedef enum {START, PLAYERS_PLAYING, HOUSE_TURN} gamePhase;

gamePhase initGame (gameTable *table);

#endif /* end include guard */
