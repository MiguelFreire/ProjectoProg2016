/**
 * @file
 *
 * Error handling goes here!
 */

#ifndef ERROR_H
#define	ERROR_H
void fireNotEnoughMemoryError(const char *s);

void fireOutOfRangeError(const char *s, float min, float max);

void fireUnknownValueError(const char *s);

void fireFormatError(const char *s);
#endif /* end include guard */
