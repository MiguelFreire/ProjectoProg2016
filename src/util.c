#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "util.h"
#include "config.h"

bool isBetween(float v, float min, float max) {
  if(v <= max && v >= min) return true;
  return false;
}

void addNullByte(char *s) {
  int size = strlen(s);
  s[size-1] = '\0';
}

void logPlay(char const *player, char const *action) {
	char buff[MAX_BUFFER_SIZE];
	time_t now = time(NULL);
	strftime(buff, MAX_BUFFER_SIZE, "[%H:%M:%S]", localtime(&now));
	buff[10] = '\0';
	printf("%s %s %s\n", buff, player, action);
}
