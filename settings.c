#include "fileIO.h"
#include "config.h"

Settings *readSettings() {
  FILE *settingsFile = fopen(SETTINGS_FILE_NAME, "r");
  if(settingsFile == NULL) {
    printf("Failed to load the settings file: %s", SETTINGS_FILE_NAME);
    exit(0);
  }

  char buffer[MAX_BUFFER_SIZE];
  Settings stg = (Settings *) malloc(sizeof(Settings)); //Don't forget to free after all settings have been read!!!
  settingsFile=readGameSettings(buffer, settingsFile, stg);
  readPlayerSettings(buffer, settingsFile, stg);

  fclose(settingsFile);
}

void readGameSettings(char *buffer, FILE *settingsFile, Settings stg) {
  fgets(buffer, MAX_BUFFER_SIZE, settingsFile);
  sscanf(buffer, "%d-%d", stg->numDecks, stg->numPlayers);
  return *settingsFile;
}

void readPlayerSettings(char *buffer, FILE *settingsFile, Settings stg) {
  while (fgets(buffer, MAX_BUFFER_SIZE, settingsFile)) {
    
  }
