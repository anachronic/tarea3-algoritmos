#ifndef TAREA3_ALGORITMOS_SPLAYTREE_H
#define TAREA3_ALGORITMOS_SPLAYTREE_H

#include "entry.h"

// Fuentes para entender los sources:
// (1): http://www.geeksforgeeks.org/splay-tree-set-1-insert/
// (2): http://algs4.cs.princeton.edu/33balanced/SplayBST.java.html

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

unsigned long long splaytree_espacio(splaytree *s);

#endif //TAREA3_ALGORITMOS_SPLAYTREE_H
