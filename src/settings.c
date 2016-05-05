#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileIO.h"
#include "config.h"
#include "players.h"
#include "settings.h"
#include "errorHandling.h"

/**
 * [readSettings description]
 * @param  players [description]
 * @return         [description]
 */
Settings readSettings(const char *fileName) {
  FILE *settingsFile = fopen(fileName, "r");
  if(settingsFile == NULL) {
    fireFileNotFoundError(fileName);
  }

  char buffer[MAX_BUFFER_SIZE];
  //sets the file stream pointing to the second line
  GameSettings gameStg= readGameSettings(buffer, settingsFile);
  PlayerSettings *playerStg= readPlayerSettings(buffer, settingsFile, gameStg.numPlayers);
  Settings stg = {gameStg, playerStg};

  fclose(settingsFile);

  return stg;
}

/**
 * [readGameSettings description]
 * @param buffer       [description]
 * @param settingsFile [description]
 * @param stg          [description]
 */
GameSettings readGameSettings(char *buffer, FILE *settingsFile) {
  GameSettings gameStg;
  fgets(buffer, MAX_BUFFER_SIZE, settingsFile);
  if(sscanf(buffer, "%d-%d", &gameStg.numDecks, &gameStg.numPlayers) == -1) {
    fireFormatError("[NumDecks]-[NumPlayers]");
  }

  return gameStg;
}

/**
 * [readPlayerSettings description]
 * @param buffer       [description]
 * @param settingsFile [description]
 * @param players      [description]
 */
PlayerSettings *readPlayerSettings(char *buffer, FILE *settingsFile, int numPlayers) {
  /*
  * TODO:
  * Create player settings struct from settings struct numPlayers
  * Pass the data from file to settings struct
  * Return that settings struct and apply to player struct in player library
  * Free the player settings struct in main!
  */
  char playerTypeChar[MAX_PLAYER_TYPE_SIZE+1];
  PlayerType  playerType;
  char name[MAX_NAME_SIZE+1];

  PlayerSettings *playerStg =(PlayerSettings *) malloc(numPlayers*sizeof(PlayerSettings));

  int i = 0;
  while (fgets(buffer, MAX_BUFFER_SIZE, settingsFile)) {
    if(sscanf(buffer,"%[^-]-%[^-]-%d-%d", playerTypeChar, name, &playerStg[i].seedMoney, &playerStg[i].seedBet) == -1) {
      fireFormatError("[PlayerType]-[PlayerName]-[SeedMoney]-[SeedBetValue]");
    }

    if(strcmp(playerTypeChar, "HU") == 0) playerType = HUMAN;
    else if(strcmp(playerTypeChar, "EA") == 0) playerType = CPU;
    else {
      fireUnknownValueError("HU or EA");
    }

    strcpy(playerStg[i].name, name);

    i++;
  }

  return playerStg;
}

void freeSettingsStruct(Settings *stg) {
    for(int i = 0; i < stg->gameStg.numPlayers; i++) {
      free(stg->playerStg);
    }
}
