#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"


bool isBetween(float v, float min, float max) {
  if(v <= max && v >= min) return true;
  return false;
}

void addNullByte(char *s) {
  int size = strlen(s);
  s[size-1] = '\0';
}
