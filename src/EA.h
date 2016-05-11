#ifndef EA_H
#define	EA_H


typedef enum {aHIT, aDOUBLE, aSURRENDER, aSTAND} eaAction;

int **readSoftEAMatrix();

int **readHardEAMatrix();

int getAction(char action);

int actionDecoder(int **softMatrix, int **hardMatrix, GameTable *table);



#endif
