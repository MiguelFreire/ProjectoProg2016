////////////////////////////////////////////////////////////////////////////////
//                                  SETTINGS.H                                //
//                                                                            //
// Game and player settings related functions                                 //
////////////////////////////////////////////////////////////////////////////////

#ifndef SETTINGS_H
#define SETTINGS_H
/**
 * Structure that has the game related settings
 */
typedef struct {
  int numDecks; // number of decks
  int numPlayers; // number of players
} GameSettings;

/**
 * Structure that has the player related settings
 */
typedef struct {
  PlayerType playerType;
  char name[MAX_NAME_SIZE+1];
  int seedMoney; // initilial money
  int seedBet; //initilial bet
} PlayerSettings;

/**
 * Structure that has all the settings (game settings and player settings)
 */
typedef struct {
  GameSettings gameStg;
  PlayerSettings *playerStg;
} Settings;

Settings readSettings(const char *fileName);

GameSettings readGameSettings(char *buffer, FILE *settingsFile);

PlayerSettings *readPlayerSettings(char *buffer, FILE *settingsFile,
     int numPlayers);

void freeSettingsStruct(Settings *stg);

#endif // end iclude guard
