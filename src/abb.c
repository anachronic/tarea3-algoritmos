#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "abb.h"


static int _nodo_es_hoja(struct abb_nodo *nodo){
  if(nodo == NULL) return 0;

  return nodo->izq == NULL && nodo->der == NULL;
}

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
    n->e->val_size = valsize;
    memcpy(n->e->val, val, valsize);

    return n;
  }
  
  int cmp = strcmp(key, nodo->e->key);
  
  if(cmp < 0)
    nodo->izq = _abb_insertar(nodo->izq, key, val, valsize);
  else if(cmp > 0)
    nodo->der = _abb_insertar(nodo->der, key, val, valsize);
  else {
    // cmp es igual a 0. El elemento ya está, lo reemplazo.
    entry_replace_val(nodo->e, val, valsize);
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


// Esto elimina un nodo que tenga hijos NULL. NO USAR para nodos internos!!!
static struct abb_nodo *_abb_eliminar_nodo(struct abb_nodo *nodo){
  struct abb_nodo *hijo;

  // Eliminar la entry de este nodo
  // y yo mismo tb digo adios al mundo
  if(nodo->e != NULL){
    free_entry(nodo->e);
    free(nodo->e);
  }

  // Ahora veo si tengo UN solo hijo
  if(nodo->izq != NULL && nodo->der == NULL){
    // solo tengo hijo izquierdo
    hijo = nodo->izq;
    free(nodo);
    return hijo;
  } else if (nodo->izq == NULL && nodo->der != NULL){
    // solo tengo hijo derecho
    hijo = nodo->der;
    free(nodo);
    return hijo;
  }

  // ASUMO que soy hoja y retorno NULL
  free(nodo);
  return NULL;
}

// Retorna el árbol en donde se ha eliminado el hijo más a la izquierda
// y se ha escrito la entry eliminada en "e".
static struct abb_nodo *_abb_borrow(struct abb_nodo *nodo, entry *e){
  if(nodo->izq != NULL){
    nodo->izq = _abb_borrow(nodo->izq, e);
    return nodo;
  }

  // no tengo más hijos izquierdos.
  entry_new(e, nodo->e->key, nodo->e->val, nodo->e->val_size);

  return _abb_eliminar_nodo(nodo);
}

static struct abb_nodo *_abb_eliminar(struct abb_nodo *nodo, const char *key){
  if(nodo == NULL) return NULL;

  // tengo que buscar el nodo donde debo eliminar
  int cmp = strcmp(key, nodo->e->key);
  
  if (cmp > 0) {
    // busco en el subárbol derecho
    nodo->der = _abb_eliminar(nodo->der, key);
    return nodo;
  } else if (cmp < 0) {
    // buscar en el subárbol izquierdo
    nodo->izq = _abb_eliminar(nodo->izq, key);
    return nodo;
  }
  // cmp == 0
  // lo pillé en este nodo.
  // si es hoja, lo elimino y se acaba el cuento.
  // si es interno, tenemos 2 casos
  //  - si tiene 1 solo hijo eliminamos directamente
  //  - si tiene 2 hijos -> borrow.
  if (_nodo_es_hoja(nodo)) return _abb_eliminar_nodo(nodo);
  else if ((nodo->izq == NULL && nodo->der != NULL) ||
           (nodo->izq != NULL && nodo->der == NULL)){
    // tengo 1 solo hijo, elimino directametne
    return _abb_eliminar_nodo(nodo);
  }

  // llegamos al peor caso: tenemos 2 hijos.
  // Primero, borro lo que hay en la entry de este nodo.
  free_entry(nodo->e);
  free(nodo->e);

  nodo->e = (entry*)malloc(sizeof(entry));

  // y devuelvo borrow de mi subárbol derecho que siempre existe
  nodo->der = _abb_borrow(nodo->der, nodo->e);
  return nodo;
}

void abb_eliminar(abb *a, const char *key){
  a->raiz = _abb_eliminar(a->raiz, key);
}



static void _free_abb(struct abb_nodo *nodo){
  if(nodo == NULL) return;

  _free_abb(nodo->izq);
  _free_abb(nodo->der);

  free_entry(nodo->e);
  free(nodo->e);
  free(nodo);
}


void abb_dispose(abb *a){
  _free_abb(a->raiz);
  a->raiz = NULL;
}

static unsigned long long _abb_espacio(struct abb_nodo *nodo){
  if(nodo == NULL) return 0;

  //este es el peso de esta estructura;
  unsigned long long tama_estructura = sizeof(struct abb_nodo) + entry_size(nodo->e);

  // el peso total es el peso de esta struct + el peso de los 2 subárboles.
  return tama_estructura + _abb_espacio(nodo->izq) + _abb_espacio(nodo->der);
}

unsigned long long abb_espacio(abb *a){
  return sizeof(abb) + _abb_espacio(a->raiz);
}
