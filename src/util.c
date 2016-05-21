////////////////////////////////////////////////////////////////////////////////
//                                  UTIL.C                                    //
//                                                                            //
// Helper functions                                                           //
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "players.h"
#include "cards.h"
#include "settings.h"
#include "EA.h"
#include "util.h"

/**
 * @brief      Checks if a given variable is between two values
 *
 * @param      v - variable to test
 * @param      min - minimum value to test: v >= min
 * @param      max - maximum value to test: v <= max
 *
 * @return     bool
 */
bool isBetween(float v, float min, float max) {
  if(v <= max && v >= min) return true;
  return false;
}

/**
 * @brief      Logs the plays and actions happening in the game
 *
 * @param      *player - string with the player name
 * @param      *action - string with the action/play name
 *
 * @return     void
 */
void logPlay(char const *player, char const *action) {
	char buff[MAX_BUFFER_SIZE];
	time_t now = time(NULL);
	strftime(buff, MAX_BUFFER_SIZE, "[%H:%M:%S]", localtime(&now));
	buff[10] = '\0';
	printf("%s %s %s\n", buff, player, action);
}

/**
 * @brief      Clears the terminal
 *
 * @return     void
 */
void clearTerminal(){
	printf("\033[2J\033[1;1H");
}


/**
 * @brief      Frees all dinamically allocated memory
 *
 * @param      playerList  ptr to the player list structure
 * @param      house       ptr to the house structure
 * @param      cardPile    ptr to the card pile structure
 * @param      settings    ptr to the settings structure
 * @param      softMatrix  the EA soft hand matrix
 * @param      hardMatrix  the EA hard hand matrix
 */
void freeEverything(PlayerList *playerList, House *house, Pile *cardPile, Settings *settings, int **softMatrix, int **hardMatrix){
	CardNode *tmpCard = NULL;
	// free players
	while (playerList->head != NULL){
		// remove player frees the player hand and the player
		playerList->head = removePlayer(playerList);
	}
	// free house hand
	tmpCard = house->hand;
	while (tmpCard != NULL){
		tmpCard = popHand(tmpCard, NULL, &house->numCards);
	}
	// free card pile

	while (cardPile->pileTop != NULL){
		cardPile->pileTop = removeCardFromTop(cardPile);
	}
	//free EA matrixes
	freeMatrixes(softMatrix, hardMatrix);

	// free settings
	freeSettingsStruct(settings);
}
