#ifndef diff_h
#define diff_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define VERSION_NUM 1.12
#define ON 1
#define OFF 0
#define STRING_MAX 4096

void print_version(void);
void loadfiles(const char* filename1, const char* filename2);
void perform_flags(int argc, const char* argv[]);
void check_flags(int argc, const char* argv[]);
void set_flags(const char* arg, const char* s, const char* t, int* value);
int files_check(int argc, const char* argv[]);

char buf[BUFSIZ];
char *strings1[STRING_MAX], *strings2[STRING_MAX];
char *differ[STRING_MAX][2], *same[STRING_MAX][2];
//Set initial state of flags and count
int version = OFF, brief = OFF, identical_files = OFF, identical_or_different_files = OFF, _normal = OFF,
    side_by_side = OFF, left = OFF, suppress_common = OFF, _context = OFF, _unified = OFF, different = OFF,
    count1 = 0, count2 = 0, differ_index = 0, same_index = 0;
#endif
