#ifndef _EXPERIMENTOS_H_
#define _EXPERIMENTOS_H_

#include "cadenas.h"

#define STDEV_PREALLOC 200

struct stdev_estadistica {
  unsigned long long *valores;
  int n;
  int allocLen;
};

void stdev_new(struct stdev_estadistica *se);
void stdev_add(struct stdev_estadistica *se, unsigned long long val);
void stdev_free(struct stdev_estadistica *se);

void experimento(const char *estructura, const char *espacio, const char *tipoinput);
void experimento_splay(const char *espacio);
void experimento_avl(const char *espacio);

#endif // _EXPERIMENTOS_H_
