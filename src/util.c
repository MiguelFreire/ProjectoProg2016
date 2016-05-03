#include "util.h"

bool isBetween(float v, float max, float min) {
  if(v < max && v > min) return true;
  return false;
}

void addNullByte(char *s) {
  int size = strlen(s);
  s[size-1] = '\0';
}
