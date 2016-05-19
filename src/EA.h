#ifndef EA_H
#define	EA_H

#include "cards.h"
#include "gameMechanics.h"

int increaseEADelay(int delayLevel);
int decreaseEADelay(int delayLevel);

void readEAMatrix(const char *fileName, int **softMatrixArg, int **hardMatrixArg);

int getAction(char action);

int actionDecoder(int **softMatrix, int **hardMatrix, GameTable *table);

void freeMatrixes(int **softMatrix, int **hardMatrix);

int HiLoCounter(CardNode *node);


#endif
