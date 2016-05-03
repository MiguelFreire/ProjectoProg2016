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
  char playerType[MAX_PLAYER_TYPE_SIZE];
  int  playerTypeInt;
  char name[MAX_NAME_SIZE];

  int i = 0;
  while (fgets(buffer, MAX_BUFFER_SIZE, settingsFile)) {
    sscanf(buffer,"%s-%s-%d-%d", playerType, name, &(players[i].money), &(players[i].bet));
    if(strcmp(playerType, "HU") == 0) playerTypeInt = 0;
    else if(strcmp(playerType, "EA") == 1) playerTypeInt = 1;
    else {
      char types[] = {"HU", "EA"};
      fireUnknownValue("player type", types, 2);
    }
    *(players[i].type) = playerTypeInt;
    strcpy(name, player[i].name);

    i++;
  }
}
