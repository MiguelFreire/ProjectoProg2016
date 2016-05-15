#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "config.h"
#include "errorHandling.h"
#include "util.h"
#include "gameMechanics.h"
#include "EA.h"

int increaseEADelay(int delayLevel){
    if (delayLevel < 5){
        return delayLevel + 1;
    } else {
        return delayLevel;
    }
}

int decreaseEADelay(int delayLevel){
    if (delayLevel > 1){
        return delayLevel - 1;
    } else {
        return delayLevel;
    }
}

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
            break;
        case 'D':
            return aDOUBLE;
            break;
        case 'S':
            return aSTAND;
            break;
        case 'R':
            return aSURRENDER;
            break;
        default:
            return -1;
    }
}

int actionDecoder(int **softMatrix, int **hardMatrix, GameTable *table) {
    CardNode *playerHand =  table->slots[table->currentPlayer]->player.hand;
    CardNode *houseHand = table->house->hand;

    int houseCard = houseHand->next->card.value;
    int playerHandValue = table->slots[table->currentPlayer]->player.handValue;

    int col = houseCard - 2;
    int row = 0;

    if(hasAces(playerHand, 2) > 0) {
        //soft
        if(playerHandValue >= 19) row = 6;
        else row = playerHandValue - 13;
        printf ("row: %d, colummn: %d\n", row, col);
        return softMatrix[row][col];
    } else {
        if(playerHandValue >= 17) row = 9;
        else if(isBetween(playerHandValue, 4,8)) row = 0;
        else row = playerHandValue - 8;
        printf ("row: %d, colummn: %d\n", row, col);
        return hardMatrix[row][col];
    }
}

int HiLoCounter(CardNode *node) {
    int HiLoMatrix[CARD_RANKS] = {1,1,1,1,1,0,0,0,-1,-1,-1,-1,-1};

    int value = HiLoMatrix[node->card.rank];

    return value;
}
