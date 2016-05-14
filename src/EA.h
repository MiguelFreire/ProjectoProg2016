#ifndef EA_H
#define	EA_H

#include "cards.h"
#include "gameMechanics.h"


int **readSoftEAMatrix();

int **readHardEAMatrix();

int getAction(char action);

int actionDecoder(int **softMatrix, int **hardMatrix, GameTable *table);

int HiLoCounter(CardNode *node);


#endif
