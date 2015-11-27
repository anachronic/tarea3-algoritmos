#ifndef TAREA3_ALGORITMOS_ABB_H
#define TAREA3_ALGORITMOS_ABB_H

#include "entry.h"

typedef struct {
  struct abb_nodo *raiz;
} abb;

struct abb_nodo {
  entry *e;
  struct abb_nodo *izq;
  struct abb_nodo *der;
};

void abb_new(abb *a);
void abb_insertar(abb *a, const char *key, void *val, int valsize);
void* abb_buscar(abb *a, const char *key);
void abb_eliminar(abb *a, const char *key);
void abb_dispose(abb *a);



#endif //TAREA3_ALGORITMOS_ABB_H
