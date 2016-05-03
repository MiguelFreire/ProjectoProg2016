#include "fileIO.h"
#include "config.h"
#include "players.h"

Settings readSettings() {
  FILE *settingsFile = fopen(SETTINGS_FILE_NAME, "r");
  if(settingsFile == NULL) {
    printf("Failed to load the settings file: %s", SETTINGS_FILE_NAME);
    exit(0);
  }

  char buffer[MAX_BUFFER_SIZE];
  Settings stg;
  readGameSettings(buffer, settingsFile, *stg); //sets the file stream pointing to the second line
  readPlayerSettings(buffer, settingsFile, *stg);

  fclose(settingsFile);
}

void readGameSettings(char *buffer, FILE *settingsFile, Settings *stg) {
  fgets(buffer, MAX_BUFFER_SIZE, settingsFile);
  sscanf(buffer, "%d-%d", stg->numDecks, stg->numPlayers);
}

void readPlayerSettings(char *buffer, FILE *settingsFile, Player *players) {
  char playerType[2];
  int  playerTypeInt;
  char name[8];


  int i = 0;
  while (fgets(buffer, MAX_BUFFER_SIZE, settingsFile)) {
    sscanf(buffer,"%s-%s-%d-%d", playerType, name, &(players[i].money), &(players[i].bet));

    playerType[MAX_PLAYER_TYPE_SIZE] = '\0'; //replaces the new line operator with the null byte terminator for strcmp
    name[MAX_NAME_SIZE] = '\0';

    // if(strcmp(playerType, "HU") == 0) &(playerType)
    strcpy(playerType, player[i].name);
    i++;
  }
}
