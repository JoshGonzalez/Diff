#ifndef para_h
#define para_h

#include <stdio.h>

typedef struct{
  char** base;
  int filesize;
  int start;
  int stop;
  char* firstline;
  char* secondline;
}para;

para* para_make(char* base[], int size, int start, int stop);
para* para_first(char* base[], int size);
para* para_next(para* p);
size_t para_filesize(para* p);
size_t para_size(para* p);
char** para_base(para* p);
char* para_info(para* p);
int para_equal(para* p, para* q);
//void para_print(para* p, const char character, void (*fp)(const char*, const char));
void para_printfile(char* base[], int count, void (*fp)(const char*));

void normal(para* p, para* q);
void sidebyside(para* p, para* q);
void leftcolumn(para* p, para* q);
void suppresscommon(para* p, para* q);
void context(para* p, para* q);
void unified(para* p, para* q);
#endif
