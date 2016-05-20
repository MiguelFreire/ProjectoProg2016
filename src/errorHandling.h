////////////////////////////////////////////////////////////////////////////////
//                              ERRORHANDLING.H                               //
//                                                                            //
// Funtions used to print messages to terminal when an error occurs in the    //
// program                                                                    //
////////////////////////////////////////////////////////////////////////////////

#ifndef ERROR_H
#define	ERROR_H
void fireNotEnoughMemoryError(const char *s);

void fireOutOfRangeError(const char *s, float min, float max);

void fireUnknownValueError(const char *s);

void fireFormatError(const char *s);

void fireFileNotFoundError(const char *s);

void fireFileIsEmptyError(const char *s);

void fireMissingFileError(const char *s);
#endif /* end include guard */
