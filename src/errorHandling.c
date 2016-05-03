#include "errorHandling.h"

void fireNotEnoughMemory(const char *s) {
  printf("There is not enough memory to allocate the object: %s\n", s);
}

void fireOutOfRange(const char *s, float min, float max) {
  printf("%s must be between: %.2f and %.2f\n", s, min, max);
}
void fireUnknownValue(const char *s, char **types, int numTypes) {
  printf("The value of %s is not supported\n", s);
  printf("Available values for %s\n", s);
  for(int i = 0; i <= numTypes; i++) {
    prinft("- %s\n", s[i]);
  }
}
