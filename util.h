#ifndef util_h
#define util_h

#include <stdio.h>

char* yesorno(int condition);
FILE* openfile(const char* filename, const char* openflags);
void printline(void);
void printleft(const char* left);
void printleft_prefix(const char* left, const char prefix);
void printleft_suffix(const char* left, const char suffix);
void printright(const char* right, const char character);
void printboth(const char* left_right, const char filler);
#endif
