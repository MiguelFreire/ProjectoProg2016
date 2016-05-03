/**
 * @file
 *
 * Error handling goes here!
 */

#ifndef ERROR_H
#define	ERROR_H

void fireNotEnoughMemory(const char *s);

void fireOutOfRange(const char *s, float min, float max);

void fireUnknownValue(const char *s, char **types, int numTypes);
#endif /* end include guard */
