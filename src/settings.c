////////////////////////////////////////////////////////////////////////////////
//                                SETTINGS.C                                  //  
//                                                                            //
// Game and player settings related functions                                 //
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "config.h"
#include "players.h"
#include "settings.h"
#include "errorHandling.h"
#include "util.h"

/**
 * @brief      Calls readGameSettings and readPlayerSettings and creates a
 *             settings structure
 *
 * @param      *fileName - the name of the settings file
 *
 * @return     Settings
 */
 Settings readSettings(const char *fileName) {
     FILE *settingsFile = fopen(fileName, "r");
     if(settingsFile == NULL) fireMissingFileError("Game Settings");


     char buffer[MAX_BUFFER_SIZE];
     //sets the file stream pointing to the second line
     GameSettings gameStg= readGameSettings(buffer, settingsFile);
     PlayerSettings *playerStg= readPlayerSettings(buffer, settingsFile, gameStg.numPlayers);
     Settings stg = {gameStg, playerStg};

     fclose(settingsFile);

     return stg;
 }

 /**
  * @brief      Reads the game settings from file and creates a GameSettings
  *             structure
  *
  * @param      *buffer - file stream buffer
  * @param      *settingsFile - file pointer
  *
  * @return     void
  */
 GameSettings readGameSettings(char *buffer, FILE *settingsFile) {
     int numDecks=0, numPlayers=0;
     if(fgets(buffer, MAX_BUFFER_SIZE, settingsFile) == NULL) {
         fireFileIsEmptyError("Settings file");
     }
     if(sscanf(buffer, "%d-%d", &numDecks, &numPlayers) == -1) {
         fireFormatError("[NumDecks]-[NumPlayers]");
     }

     if(!isBetween(numDecks, MIN_DECKS, MAX_DECKS)) fireOutOfRangeError("number of decks", MIN_DECKS, MAX_DECKS);
     if(!isBetween(numPlayers, MIN_PLAYERS, TABLE_SLOTS)) fireOutOfRangeError("number of players",MIN_PLAYERS,TABLE_SLOTS);

     GameSettings gameStg = {numDecks, numPlayers};

     return gameStg;
 }

 /**
  * @brief      Reads the player settings from file and creates a PlayerSettings
  *             structure
  *
  * @param      *buffer - file stream buffer
  * @param      *settingsFile - file pointer
  * @param      numPlayers - number of players
  *
  * @return     void
  */
PlayerSettings *readPlayerSettings(char *buffer, FILE *settingsFile, int numPlayers) {

  char playerTypeChar[MAX_PLAYER_TYPE_SIZE+1];
  PlayerType  playerType;
  char name[MAX_NAME_SIZE+1];
  int seedMoney = 0;
  int seedBet = 0;

  PlayerSettings *playerStg =(PlayerSettings *) malloc(numPlayers*sizeof(PlayerSettings));

  int i = 0;
  while (fgets(buffer, MAX_BUFFER_SIZE, settingsFile)) {
    if(sscanf(buffer,"%[^-]-%[^-]-%d-%d", playerTypeChar, name, &seedMoney, &seedBet) == -1) {
      fireFormatError("[PlayerType]-[PlayerName]-[SeedMoney]-[SeedBetValue]");
    }

    if(!isBetween(seedMoney, 10, 500)) fireOutOfRangeError("seed money", 10, 500);
    if(!isBetween(seedBet, 2, 0.25*seedMoney)) fireOutOfRangeError("seed money", 2, 0.25*seedMoney);


    if(strcmp(playerTypeChar, "HU") == 0) playerType = HUMAN;
    else if(strcmp(playerTypeChar, "EA") == 0) playerType = CPU;
    else {
      fireUnknownValueError("HU or EA");
    }

    playerStg[i].playerType = playerType;
    playerStg[i].seedMoney = seedMoney;
    playerStg[i].seedBet = seedBet;
    strcpy(playerStg[i].name, name);

    i++;
  }

  return playerStg;
}
/**
 * @brief      Frees the settings struct dynamic memory
 *
 * @param      *stg - settings structure pointer
 *
 * @return     void
 */
void freeSettingsStruct(Settings *stg) {
    free(stg->playerStg);
}
