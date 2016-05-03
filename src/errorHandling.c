#include <stdlib.h>
#include <stdio.h>

#include "errorHandling.h"

void fireNotEnoughMemoryError(const char *s) {
  printf("There is not enough memory to allocate the object: %s\n", s);
  exit(EXIT_FAILURE);
}

void fireOutOfRangeError(const char *s, float min, float max) {
  printf("%s must be between: %.2f and %.2f\n", s, min, max);
  exit(EXIT_FAILURE);
}
void fireUnknownValueError(const char *s) {
  printf("The value is not supported! Valid values: %s\n", s);
  exit(EXIT_FAILURE);
}

void fireFormatError(const char *s) {
  printf("Invalid format!\n Valid Format: %s\n", s);
  exit(EXIT_FAILURE);
}
