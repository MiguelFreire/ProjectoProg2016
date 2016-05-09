/**
 * Program flow functions
 */

#ifndef FILEIO_H
#define	FILEIO_H

#include "gameMechanics.h"
#include "settings.h"

typedef enum {START, PLAYERS_PLAYING, HOUSE_TURN} GamePhase;

GamePhase initGame (GameTable *table, PlayerList *playerList, Pile *pile, Settings *settings, char *argv1);

void freeEverything(PlayerList *playerList, House *house, Pile *cardPile, Settings *settings);

#endif /* end include guard */
