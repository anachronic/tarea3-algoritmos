#include <string.h>
#include <stdlib.h>

#include "avl.h"


void avl_new(avl *a){
  a->raiz = NULL;
}


void avl_insertar(avl* a, const char *key, void *val, int valsize) {
  return;
}


static void *_avl_buscar(struct avl_nodo *nodo, const char *key){
  if(nodo == NULL) return NULL;

  int cmp = strcmp(key, nodo->e->key);

  if(cmp < 0) return _avl_buscar(nodo->izq, key); // por la izquierda
  else if (cmp > 0) return _avl_buscar(nodo->der, key); // por la derecha

  // cmp == 0;
  return nodo->e->val;
}

void *avl_buscar(avl* a, const char *key) {
  return _avl_buscar(a->raiz, key);
}


