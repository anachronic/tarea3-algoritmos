#include <string.h>
#include <stdlib.h>

#include "avl.h"


void avl_new(avl *a){
  a->raiz = NULL;
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


static struct avl_nodo *_avl_insertar(struct avl_nodo *nodo, const char *key, void *val, int valsize, int altura){
  if (nodo == NULL) {
    struct avl_nodo *n = malloc(sizeof(struct avl_nodo));
    n->der = NULL;
    n->izq = NULL;
    n->e = malloc(sizeof(entry));
    entry_new(n->e, key, val, valsize);
    n->altura = altura;
    return n;
  }

  
}

void avl_insertar(avl* a, const char *key, void *val, int valsize) {
  
}
