////////////////////////////////////////////////////////////////////////////////
//                                      EA.H                                  //
//                                      									  //
// EA config matrix file reading, EA decision making and delay control        //
////////////////////////////////////////////////////////////////////////////////

#ifndef EA_H
#define	EA_H

#include "cards.h"
#include "gameMechanics.h"

int increaseEASpeed(int *speed);
int decreaseEASpeed(int *speed);

void readEAMatrix(const char *fileName, int ***softMatrixArg, int ***hardMatrixArg);

int getAction(char action);

int actionDecoder(int **softMatrix, int **hardMatrix, GameTable *table);

void freeMatrixes(int **softMatrix, int **hardMatrix);


#endif
