typedef struct {
  int numDecks;
  int numPlayers;
} Settings;

Settings readSettings();
void readGameSettings(char *buffer, FILE *settingsFile, Settings *stg);
void readPlayerSettings(char *buffer, FILE *settingsFile, Player *players);
