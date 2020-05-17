#include <string.h>
#include <stdlib.h>
#include "util.h"


char* yesorno(int condition) { return condition == 0 ? "no" : "YES"; }

FILE* openfile(const char* filename, const char* openflags) {
    FILE* f;
    if ((f = fopen(filename, openflags)) == NULL) {  printf("can't open '%s'\n", filename);  exit(1); }
    return f;
}

void printline(void) {
    for (int i = 0; i < 10; ++i) { printf("=========="); }
    printf("\n");
}

void printleft_suffix(const char* left, const char suffix) {
  char buf[BUFSIZ];
  strcpy(buf, left);
  int j = 0, len = (int)strlen(buf) - 1;
  for (j = 0; j <= 48 - len ; ++j) { buf[len + j] = ' '; }
  buf[len + j++] = suffix;
  buf[len + j++] = '\0';
  printf("%s\n", buf);
}

void printleft_prefix(const char* left, const char prefix) {
  char buf[BUFSIZ];
  strcpy(buf, left);
  int j = 0, len = (int)strlen(buf) - 1;
  for (j = 0; j <= 48 - len ; ++j) { buf[len + j] = ' '; }
  buf[len + j++] = '\0';
  printf("%c %s\n", prefix, buf);
}

void printright(const char* right, const char character) {
  if (right == NULL) { return; }
  printf("%50c %s", character, right);
}

void printboth(const char* left_right, const char filler) {
  char buf[BUFSIZ];
  size_t len = strlen(left_right);
  if (len > 0) { strncpy(buf, left_right, len); }
  buf[len - 1] = '\0';
  printf("%-50s%c%s", buf, filler, left_right);
}
