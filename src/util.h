////////////////////////////////////////////////////////////////////////////////
//                                  UTIL.H                                    //
//                                                                            //
// Helper functions                                                           //
////////////////////////////////////////////////////////////////////////////////

#ifndef UTIL_H
#define	UTIL_H

#include "players.h"
#include "cards.h"
#include "settings.h"

bool isBetween(float v, float min, float max);

void addNullByte(char *s);

void logPlay(char const *, char const *);

void clearTerminal();

void freeEverything(PlayerList *playerList, House *house, Pile *cardPile, 
	Settings *settings, int **softMatrix, int **hardMatrix);

#endif /* end include guard */
