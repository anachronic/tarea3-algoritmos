#ifndef TAREA3_ALGORITMOS_VANEMDEBOAS_H
#define TAREA3_ALGORITMOS_VANEMDEBOAS_H

#include "entry.h"

#define VANEMDEBOAS_UNIVERSO (1<<31)

// Fuentes para entender el source
// (1): http://www.daimi.au.dk/~gudmund/dynamicF04/vEB.pdf
// (2): Apuntes del profe

typedef struct {
  struct vebtree *veb;
} vanemdeboas;

struct vebtree {
  unsigned int size;
  unsigned int max;
  unsigned int min;
  struct vebtree *top;
  struct vebtree **bottom;
  unsigned int wordsize;
};

void vanemdeboas_new(vanemdeboas* veb);
void vanemdeboas_insertar(vanemdeboas* veb, const char *key, void *val, int valsize);
void *vanemdeboas_buscar(vanemdeboas *veb, const char *key);
void vanemdeboas_eliminar(vanemdeboas *veb, const char *key);
void vanemdeboas_dispose(vanemdeboas *veb);


#endif //TAREA3_ALGORITMOS_VANEMDEBOAS_H
