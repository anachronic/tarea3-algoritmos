#ifndef TAREA3_ALGORITMOS_VANEMDEBOAS_H
#define TAREA3_ALGORITMOS_VANEMDEBOAS_H

#include "entry.h"

// Fuentes para entender el source
// (1): http://www.daimi.au.dk/~gudmund/dynamicF04/vEB.pdf
// (2): Apuntes del profe

typedef struct {
  struct vebtree *veb;
} vanemdeboas;

struct vebtree {
  unsigned int size;
  //int max;
  //int min;
  struct vebtree *top;
  struct vebtree **min;
};

void vanemdeboas_new(vanemdeboas* veb);
void vanemdeboas_insertar(vanemdeboas* veb);
void vanemdeboas_buscar(vanemdeboas *veb, const char *key);
void vanemdeboas_eliminar(vanemdeboas *veb, const char *key);
void vanemdeboas_dispose(vanemdeboas *veb);


#endif //TAREA3_ALGORITMOS_VANEMDEBOAS_H
