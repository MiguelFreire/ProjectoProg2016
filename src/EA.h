////////////////////////////////////////////////////////////////////////////////
//                                      EA.H                                  //
//                                      									  //
// EA config matrix file reading, EA decision making and delay control        //
////////////////////////////////////////////////////////////////////////////////

#ifndef EA_H
#define	EA_H

#include "cards.h"
#include "gameMechanics.h"

// Delay control functions
int increaseEASpeed(int *speed);
int decreaseEASpeed(int *speed);

// Matrix Funtions
void readEAMatrix(const char *fileName, int ***softMatrixArg, int ***hardMatrixArg);
int getAction(char action);
void freeMatrixes(int **softMatrix, int **hardMatrix);

// EA Decision Funtions
int actionDecoder(int **softMatrix, int **hardMatrix, GameTable *table);
int EAMakeDecision(SDL_Window *window, int **softMatrix, int **hardMatrix, 
	GameTable *table, Pile *pile);




#endif
