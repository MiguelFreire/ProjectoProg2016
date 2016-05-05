/**
 * Settings fecth
 */

#ifndef SETTINGS_H
#define SETTINGS_H

typedef struct {
  int numDecks;
  int numPlayers;
} GameSettings;

typedef struct {
  PlayerType playerType;
  char name[MAX_NAME_SIZE+1];
  int seedMoney;
  int seedBet;
} PlayerSettings;

typedef struct {
  GameSettings gameStg;
  PlayerSettings *playerStg;
} Settings;

Settings readSettings(const char *fileName);
GameSettings readGameSettings(char *buffer, FILE *settingsFile);
PlayerSettings *readPlayerSettings(char *buffer, FILE *settingsFile, int numPlayers);

#endif // end iclude guard
