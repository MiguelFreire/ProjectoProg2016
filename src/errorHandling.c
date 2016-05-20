#include <stdlib.h>
#include <stdio.h>

#include "errorHandling.h"

/**
 * @brief      Fires a "not enough memory" error and exits the program
 *             when there is no memory to allocate some element
 *
 * @param      *s - name of the element
 *
 * @return     void
 */
void fireNotEnoughMemoryError(const char *s) {
    printf("There is not enough memory to allocate the object: %s\n", s);
    exit(EXIT_FAILURE);
}

/**
 * @brief      Fires a "out of range" error and exits the program when a given
 *             variable has its value out of a given range
 *
 * @param      *s  - name of the variable
 * @param      min - minimum value of the given variable
 * @param      max - maximum value of the given variable
 *
 * @return     void
 */
void fireOutOfRangeError(const char *s, float min, float max) {
    printf("%s must be between: %.2f and %.2f\n", s, min, max);
    exit(EXIT_FAILURE);
}

/**
 * @brief      Fires a "unknown value" error and exits the program
 *             when some values are not supported
 *
 * @param      *s - string with valid values
 *
 * @return     void
 */
void fireUnknownValueError(const char *s) {
    printf("The value is not supported! Valid values: %s\n", s);
    exit(EXIT_FAILURE);
}

/**
 * @brief      Fires a format error and exits the program when an input has
 *             an invalid format
 *
 * @param      *s - string with the valid format
 *
 * @return     void
 */
void fireFormatError(const char *s) {
    printf("Invalid format!\n Valid Format: %s\n", s);
    exit(EXIT_FAILURE);
}

/**
 * @brief      Fires a "file not found" error and exits the program when a given
 *             file does not exist
 *
 * @param      *s - string with the file's name
 *
 * @return     void
 */
void fireFileNotFoundError(const char *s) {
    printf("File: %s was not found\n", s);
    exit(EXIT_FAILURE);

}

/**
 * @brief      Fires a "file is empty" error and exits the program when a given
 *             file is empty
 *
 * @param      *s - string with the file's name
 *
 * @return     void
 */
void fireFileIsEmptyError(const char *s) {
    printf("File: %s is empty\n", s);
    exit(EXIT_FAILURE);
}

/**
 * @brief      Fires a "missing file error" and exits the program when a
 *             required file is missing
 *
 * @param      *s - string with the file's name
 * 
 * @return     void
 */
void fireMissingFileError(const char *s) {
    printf("%s is missing\n",s);
    exit(EXIT_FAILURE);
}
