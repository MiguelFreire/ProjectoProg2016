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

#include "util.h"
#include "config.h"
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
