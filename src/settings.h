typedef struct {
  int numDecks;
  int numPlayers;
} Settings;

Settings readSettings();
FILE *readGameSettings(char *, FILE *, Settings *);
void readPlayerSettings(char *buffer, FILE*, Settings *);
