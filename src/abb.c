#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "abb.h"


void abb_new(abb *a){
  a->raiz = NULL;
}

static struct abb_nodo *_abb_insertar(struct abb_nodo *nodo, const char *key, void *val, int valsize){
  if(nodo == NULL){
    struct abb_nodo *n = (struct abb_nodo *)malloc(sizeof(struct abb_nodo));
    n->izq = NULL;
    n->der = NULL;
    n->e = (entry*)malloc(sizeof(entry));
    n->e->key = strdup(key);
    n->e->val = malloc(valsize);
    memcpy(n->e->val, val, valsize);

    return n;
  }
  
  int cmp = strcmp(nodo->e->key, key);
  
  if(cmp < 0)
    nodo->izq = _abb_insertar(nodo->izq, key, val, valsize);
  else if(cmp > 0)
    nodo->der = _abb_insertar(nodo->der, key, val, valsize);
  else {
    // cmp es igual a 0. El elemento ya está, return yo mismo.
    return nodo;
  }

  return nodo;
}

void abb_insertar(abb *a, const char *key, void *val, int valsize){
  if(a==NULL){
    fprintf(stderr, "La estructura especificada no corresponde a un ABB");
    exit(-1);
  }

  a->raiz = _abb_insertar(a->raiz, key, val, valsize);
}


static void *_abb_buscar(struct abb_nodo *nodo, const char *key){
  int cmp;

  if(nodo == NULL) return NULL;

  cmp = strcmp(key, nodo->e->key);

  if(cmp < 0)
    return _abb_buscar(nodo->izq, key);
  else if (cmp > 0)
    return _abb_buscar(nodo->der, key);

  // cmp es igual a 0, está aquí.
  return nodo->e->val;
}

void *abb_buscar(abb *a, const char *key){
  return _abb_buscar(a->raiz, key);
}





static void _free_abb(struct abb_nodo *nodo){
  if(nodo == NULL) return;

  _free_abb(nodo->izq);
  _free_abb(nodo->der);

  if(nodo->izq != NULL) free(nodo->izq);
  if(nodo->der != NULL) free(nodo->der);

  free_entry(nodo->e);
  free(nodo);
}


void abb_dispose(abb *a){
  _free_abb(a->raiz);
}
