////////////////////////////////////////////////////////////////////////////////
//                                      EA.C                                  //
//                                                                            //
// EA config matrix file reading, EA decision making and delay control        //
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "config.h"
#include "errorHandling.h"
#include "util.h"
#include "gameMechanics.h"
#include "EA.h"


/**
 * @brief      Increases EA speed/ decreases EA delay
 *
 * @param      speed  ptr to EA speed
 *
 * @return     new EA delay
 */
int increaseEASpeed(int *speed){
    int delay;

    if ((*speed) < SPEED_LEVELS){
        *speed += 1;
    }

    // integer division truncation is intended so there is an integer speed
    // level that corresponds to a delay equal to
    // (EA_BASE_DELAY - RENDER_DELAY)
    delay = (EA_BASE_DELAY - RENDER_DELAY) *
    (SPEED_LEVELS - *speed)/(SPEED_LEVELS/2);

    return delay;
}

/**
 * @brief      Decreases EA speed/ increses EA delay
 *
 * @param      speed  ptr to EA speed
 *
 * @return     new EA delay
 */
int decreaseEASpeed(int *speed){
    int delay;

    if ((*speed) > 0){
        *speed -= 1;
    }

    // integer division truncation is intended so there is an integer speed
    // level that corresponds to a delay equal to
    // (EA_BASE_DELAY - RENDER_DELAY)
    delay = (EA_BASE_DELAY - RENDER_DELAY) *
    (SPEED_LEVELS - *speed)/(SPEED_LEVELS/2);

    return delay;
}

/**
 * @brief      Reads the EA matrixes from file and points them to the function
 *             arguments
 *
 * @param      *fileName - a string with the file name containing the Matrixes
 * @param      ***SoftMatrixArg - a pointer to a 2D array that will point to
 *                                the soft matrix read from the config file
 * @param      ***hardMatrixArg - a pointer to a 2D array that will point to
 *                                the hard matrix read from the config file
 *
 * @return     void
 */
void readEAMatrix(const char *fileName, int ***softMatrixArg, int ***hardMatrixArg) {
    FILE *file = fopen(fileName, "r");
    char c;
    if(file == NULL) fireMissingFileError("config file");

    int **hardMatrix = (int **) malloc(10*sizeof(int*));
    if(hardMatrix == NULL) fireNotEnoughMemoryError("Hard Matrix");
    for(int l = 0; l < 10; l++) {
        if((hardMatrix[l] = (int *) malloc(10*sizeof(int))) == NULL) fireNotEnoughMemoryError("Hard Matrix element");
    }

    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            c = fgetc(file);
            if(c == '\n') c = fgetc(file);
            hardMatrix[i][j] = getAction(c);
        }
    }

    *hardMatrixArg = hardMatrix;

    //Soft Matrix reader
    int **softMatrix = (int **) malloc(7*sizeof(int*));
    if(softMatrix == NULL) fireNotEnoughMemoryError("Soft Matrix");
    for(int l = 0; l < 7; l++) {
        if((softMatrix[l] = (int *) malloc(10*sizeof(int))) == NULL) fireNotEnoughMemoryError("Soft Matrix element");
    }
    fgetc(file);
    for(int i = 0; i < 7; i++) {
        for(int j = 0; j < 10; j++) {
            c = fgetc(file);
            if(c == '\n') c = fgetc(file);
            softMatrix[i][j] = getAction(c);
        }
    }

    *softMatrixArg = softMatrix;

    fclose(file);
}

/**
 * @brief      Frees the matrixes allocated dynamic memory
 *
 * @param      **softMatrix - the 2D array that contains the allocated
 *                            soft matrix
 * @param      **hardMatrix - the 2D array that contains the allocated
 *                            hard matrix
 *
 * @return     void
 */
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

/**
 * @brief      Transform a given char from the EA config file into a specific
 *             action
 *
 * @param      action - a char from the EA config file
 *
 * @return     int/EAAction
 */
int getAction(char action) {
    switch (action) {
        case 'H':
            return aHIT;
            break;
        case 'D':
            return aDOUBLEH;
            break;
        case 'T':
            return aDOUBLES;
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

/**
 * @brief      Depending on house's hand and player's hand, actionDecoder
 *             returns the action for the EA player to execute, from the hard
 *             or from the soft matrix
 *
 * @param      **softMatrix - the EA soft matrix
 * @param      **hardMatrix - the EA hard matrix
 * @param      *table - pointer to the table structure
 *
 * @return     int/EAAction
 */
int actionDecoder(int **softMatrix, int **hardMatrix, GameTable *table) {
    CardNode *playerHand =  table->slots[table->currentPlayer]->player.hand;
    CardNode *houseHand = table->house->hand;

    int houseCard = houseHand->next->card.value;
    int playerHandValue = table->slots[table->currentPlayer]->player.handValue;

    int col = houseCard - 2;
    int row = 0;

    if(hasAces(playerHand) > 0) {
        //soft
        if(playerHandValue >= 19) row = 6;
        else if (playerHandValue == 12) row = 0; // player has two aces
        else row = playerHandValue - 13;

        return softMatrix[row][col];
    } else {
        if(playerHandValue >= 17) row = 9;
        else if(isBetween(playerHandValue, 4,8)) row = 0;
        else row = playerHandValue - 8;

        return hardMatrix[row][col];
    }
}
