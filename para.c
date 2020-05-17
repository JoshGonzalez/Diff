#include <stdlib.h>
#include <string.h>
#include "para.h"
#include "util.h"

static char buf[BUFSIZ];

para* para_make(char* base[], int filesize, int start, int stop){
  para* p = (para*) malloc(sizeof(para));
  p->base = base;
  p->filesize = filesize;
  p->start = start;
  p->stop = stop;
  p->firstline = (p == NULL || start < 0) ? NULL : p->base[start];
  p->secondline = (p == NULL || start < 0 || filesize < 2) ? NULL : p->base[start + 1];

  return p;
}

para* para_first(char* base[], int size){
  para* p = para_make(base, size, 0, -1);
  return para_next(p);
}

void para_destroy(para* p){ free(p); }

para* para_next(para* p){
  if (p == NULL || p->stop == p->filesize) { return NULL; }

  int i;
  para* pnew = para_make(p->base, p->filesize, p->stop + 1, p->stop + 1);
  for (i = pnew->start; i < p->filesize && strcmp(p->base[i], "\n") != 0; ++i) { }
  pnew->stop = i;

  if (pnew->start >= p->filesize) {
    free(pnew);
    pnew = NULL;
  }
  return pnew;
}
size_t para_filesize(para* p){ return p == NULL ? 0 : p->filesize; }

size_t para_size(para* p){ return p == NULL || p->stop < p->start ? 0 : p->stop - p->start + 1; }

char** para_base(para* p){ return p->base; }

char* para_info(para* p){
  snprintf(buf, sizeof(buf), "base: %p, filesize: %d, start: %d, stop: %d\n",
           p->base, p->filesize, p->start, p->stop);
  return buf;
}

int para_equal(para* p, para* q){
  if (p == NULL || q == NULL) { return 0; }
  if (para_size(p) != para_size(q)) { return 0; }
  if (p->start >= p->filesize || q->start >= q->filesize) { return 0; }
  int i = p->start, j = q->start, equal = 0;
  while ((equal = strcmp(p->base[i], q->base[i])) == 0 && i < p->stop && j < q->stop) { ++i; ++j; }
  return 1;
}

void para_print(para* p, const char character, void (*fp)(const char*, const char)){
  if (p == NULL) { return; }
  for (int i = p->start; i <= p->stop && i != p->filesize; ++i) { fp(p->base[i], character); }
}

/*void para_printfile(char* base[], int count, void (*fp)(const char*)){
  para* p = para_first(base, count);
  while (p != NULL) {
    para_print(p, ' ', fp);
    p = para_next(p);
  }
  printline();
}*/

void normal(para* p, para* q){
    int foundmatch = 0;
    para* qlast = q;
    while (p != NULL) {
        qlast = q;
        foundmatch = 0;
        while (q != NULL && (foundmatch = para_equal(p, q)) == 0) { q = para_next(q); }
        q = qlast;

        if (foundmatch) {
            while ((foundmatch = para_equal(p, q)) == 0) {
                para_print(q, '>', printleft_prefix);
                q = para_next(q);
                qlast = q;
            }
            para_print(q, '<', printleft_prefix);
            p = para_next(p);
            q = para_next(q);
        } else {
            para_print(p, '<', printleft_prefix);
            p = para_next(p);
        }
    }
    while (q != NULL) {
        para_print(q, '>', printleft_prefix);
        q = para_next(q);
    }
}

void sidebyside(para* p, para* q){
    int foundmatch = 0;
    para* qlast = q;
    while (p != NULL) {
        qlast = q;
        foundmatch = 0;
        while (q != NULL && (foundmatch = para_equal(p, q)) == 0) { q = para_next(q); }
        q = qlast;

        if (foundmatch) {
            while ((foundmatch = para_equal(p, q)) == 0) {
                para_print(q, '>', printright);
                q = para_next(q);
                qlast = q;
            }
            para_print(q, ' ', printboth);
            p = para_next(p);
            q = para_next(q);
        } else {
            para_print(p, '<', printleft_suffix);
            p = para_next(p);
        }
    }
    while (q != NULL) {
        para_print(q, '>', printright);
        q = para_next(q);
    }
}

void leftcolumn(para* p, para* q){
    int foundmatch = 0;
    para* qlast = q;
    while (p != NULL) {
        qlast = q;
        foundmatch = 0;
        while (q != NULL && (foundmatch = para_equal(p, q)) == 0) { q = para_next(q); }
        q = qlast;

        if (foundmatch) {
            while ((foundmatch = para_equal(p, q)) == 0) {
                para_print(q, '>', printright);
                q = para_next(q);
                qlast = q;
            }
            para_print(q, '(', printleft_suffix);
            p = para_next(p);
            q = para_next(q);
        } else {
            para_print(p, '<', printleft_suffix);
            p = para_next(p);
        }
    }
    while (q != NULL) {
        para_print(q, '>', printright);
        q = para_next(q);
    }
}

void suppresscommon(para* p, para* q){
    int foundmatch = 0;
    para* qlast = q;
    while (p != NULL) {
        qlast = q;
        foundmatch = 0;
        while (q != NULL && (foundmatch = para_equal(p, q)) == 0) { q = para_next(q); }
        q = qlast;

        if (foundmatch) {
            while ((foundmatch = para_equal(p, q)) == 0) {
                para_print(q, '>', printright);
                q = para_next(q);
                qlast = q;
            }
            p = para_next(p);
            q = para_next(q);
        } else {
            para_print(p, '<', printleft_suffix);
            p = para_next(p);
        }
    }
    while (q != NULL) {
        para_print(q, '>', printright);
        q = para_next(q);
    }
}

void context(para* p, para* q){
    int foundmatch = 0;
    para* qlast = q;
    while (p != NULL) {
        qlast = q;
        foundmatch = 0;
        while (q != NULL && (foundmatch = para_equal(p, q)) == 0) { q = para_next(q); }
        q = qlast;

        if (foundmatch) {
            while ((foundmatch = para_equal(p, q)) == 0) {
                para_print(q, '+', printleft_prefix);
                q = para_next(q);
                qlast = q;
            }
            para_print(q, ' ', printleft_prefix);
            p = para_next(p);
            q = para_next(q);
        } else {
            para_print(p, '-', printleft_prefix);
            p = para_next(p);
        }
    }
    while (q != NULL) {
        para_print(q, '+', printleft_prefix);
        q = para_next(q);
    }
}

void unified(para* p, para* q){
  int foundmatch = 0;
  para* qlast = q;
  while (p != NULL) {
      qlast = q;
      foundmatch = 0;
      while (q != NULL && (foundmatch = para_equal(p, q)) == 0) { q = para_next(q); }
      q = qlast;

      if (foundmatch) {
          while ((foundmatch = para_equal(p, q)) == 0) {
              para_print(q, '+', printleft_prefix);
              q = para_next(q);
              qlast = q;
          }
          para_print(q, '-', printleft_prefix);
          p = para_next(p);
          q = para_next(q);
      } else {
          para_print(p, '-', printleft_prefix);
          p = para_next(p);
      }
  }
  while (q != NULL) {
      para_print(q, '+', printleft_prefix);
      q = para_next(q);
  }
}
