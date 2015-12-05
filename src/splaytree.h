#ifndef TAREA3_ALGORITMOS_SPLAYTREE_H
#define TAREA3_ALGORITMOS_SPLAYTREE_H

#include "entry.h"

typedef struct {
  struct splay_nodo *raiz;
} splaytree;

struct splay_nodo
{
  entry *e;
  struct splay_nodo *izq;
  struct splay_nodo *der;
};


void splaytree_new(splaytree *s);
void splaytree_insertar(splaytree *s, const char *key, void *val, int valsize);
void *splaytree_buscar(splaytree *s, const char *key);
void splaytree_eliminar(splaytree *s, const char *key);
void splaytree_dispose(splaytree *s);

#endif //TAREA3_ALGORITMOS_SPLAYTREE_H
