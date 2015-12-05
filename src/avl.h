#ifndef TAREA3_ALGORITMOS_AVL_H
#define TAREA3_ALGORITMOS_AVL_H

#include "entry.h"

// patrón Composite. l0l
typedef struct {
  struct avl_nodo *raiz;
} avl;

struct avl_nodo
{
  entry *e;
  struct avl_nodo *izq;
  struct avl_nodo *der;
  int altura;
};

void avl_new(avl *a);
void avl_insertar(avl *a, const char *key, void *val, int valsize);
void* avl_buscar(avl *a, const char *key);
void avl_eliminar(avl *a, const char *key);
void avl_dispose(avl *a);

#endif //TAREA3_ALGORITMOS_AVL_H
