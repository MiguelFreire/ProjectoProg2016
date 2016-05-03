#include "util.h"

bool isBetween(float v, float max, float min) {
  if(v < max && v > min) return true;
  return false;
}
