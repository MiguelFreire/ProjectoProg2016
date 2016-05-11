#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "errorHandling.h"
#include "gameMechanics.h"
#include "EA.h"

int **readSoftEAMatrix() {
    FILE *file = fopen(SOFT_EA_MATRIX, "r");
    if(file == NULL) fireFileNotFoundError(SOFT_EA_MATRIX);

    int **matrix = (int **) malloc(7*sizeof(int*));
    if(matrix == NULL) fireNotEnoughMemoryError("Hard Matrix");
    for(int l = 0; l < 7; l++) {
        if((matrix[l] = (int *) malloc(10*sizeof(int))) == NULL) fireNotEnoughMemoryError("Soft Matrix element");
    }

    for(int i = 0; i < 7; i++) {
        for(int j = 0; j < 10; j++) {
            matrix[i][j] = getAction(fgetc(file));
        }
    }
    fclose(file);
    return matrix;
}

int **readHardEAMatrix() {
    FILE *file = fopen(HARD_EA_MATRIX, "r");
    if(file == NULL) fireFileNotFoundError(HARD_EA_MATRIX);

    int **matrix = (int **) malloc(10*sizeof(int*));
    if(matrix == NULL) fireNotEnoughMemoryError("Hard Matrix");
    for(int l = 0; l < 10; l++) {
        if((matrix[l] = (int *) malloc(10*sizeof(int))) == NULL) fireNotEnoughMemoryError("Hard Matrix element");
    }

    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            matrix[i][j] = getAction(fgetc(file));
        }
    }
    fclose(file);
    return matrix;
}

void freeMatrixes(int **softMatrix, int **hardMatrix) {
    for(int i = 0; i < 7; i ++) {
        free(softMatrix[i]);
    }
    free(softMatrix);

    for(int j = 0; j < 10; j++) {
        free(hardMatrix[j]);
    }
    free(hardMatrix);
}

int getAction(char action) {
    switch (action) {
        case 'H':
            return aHIT;
        case 'D':
            return aDOUBLE;
        case 'S':
            return aSTAND;
        case 'R':
            return aSURRENDER;
        default:
            return -1;
    }
}

EAAction actionDecoder(int **softMatrix, int **hardMatrix, GameTable *table) {
    CardNode *playerHand =  table->slots[table->currentPlayer]->player.hand;
    CardNode *houseHand = table->house->hand;
    int numCards = table->slots[table->currentPlayer]->player.numCards;
    int **matrix;
    (hasAces(playerHand, 2) > 0) ? matrix = softMatrix : matrix = hardMatrix;

    int houseCard = houseHand->card.rank;

    int playerHandValue = getHandValue(playerHand, numCards);

    int col = houseCard - 1;
    int row = 0;
    if(isBetween(playerHandValue, 4, 8)) row = 0;
    else row = playerHandValue - 8;

    return matrix[row][col];


}
