#include <string.h>
#include <stdlib.h>

#include "avl.h"

/* AVL NEW:: Nada nuevo. Inicializa un AVL */
void avl_new(avl *a){
  a->raiz = NULL;
}

/*
  Busca un elemento dentro de un AVL
*/
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

/*
  ================ Inicio funciones auxiliares de inserción y balance
*/
static int _altura_nodo(struct avl_nodo *nodo){
  if (nodo == NULL) return 0;
  return nodo->altura;
}

static int _factor_balance(struct avl_nodo *nodo){
  return _altura_nodo(nodo->der) - _altura_nodo(nodo->izq);
}

static void _fix_altura(struct avl_nodo *nodo){
  int hizq = _altura_nodo(nodo->izq);
  int hder = _altura_nodo(nodo->der);

  nodo->altura = (hizq > hder ? hizq : hder) + 1;
}

// Directamente desde http://kukuruku.co/hub/cpp/avl-trees
// Retorna la nueva raíz al hacer una rotación horario sobre nodo.
static struct avl_nodo *_rotarHorario(struct avl_nodo *nodo){
  struct avl_nodo *nueva_raiz;

  nueva_raiz = nodo->izq;
  nodo->izq = nueva_raiz->der;
  // nodo->der no cambia en horario ni en antihorario.

  nueva_raiz->der = nodo;
  _fix_altura(nodo);
  _fix_altura(nueva_raiz);

  return nueva_raiz;
}

// Retorna la nueva raíz al hacer una rotación antihorario sobre nodo.
static struct avl_nodo *_rotarAntihorario(struct avl_nodo *nodo){
  struct avl_nodo *nueva_raiz;

  nueva_raiz = nodo->der;
  nodo->der = nueva_raiz->izq;
  
  nueva_raiz->izq = nodo;
  // nueva_raiz no cambia su hijo derecho

  _fix_altura(nodo);
  _fix_altura(nueva_raiz);
  
  return nueva_raiz;
}

// Retorna el nodo balanceado
static struct avl_nodo *_balancear(struct avl_nodo *nodo){
  _fix_altura(nodo);

  if(_factor_balance(nodo) == 2){
    if(_factor_balance(nodo->der) < 0)
      nodo->der = _rotarHorario(nodo->der);
    return _rotarAntihorario(nodo);
  }

  if(_factor_balance(nodo) == -2){
    if(_factor_balance(nodo->izq) > 0)
      nodo->izq = _rotarAntihorario(nodo->izq);
    return _rotarHorario(nodo);
  }

  // no hay desbalance.
  return nodo;
}


// Retorna la raíz del AVL tras haber insertado (key, val) en nodo.
static struct avl_nodo *_avl_insertar(struct avl_nodo *nodo, const char *key, void *val, int valsize){
  if (nodo == NULL) {
    struct avl_nodo *n = malloc(sizeof(struct avl_nodo));
    n->der = NULL;
    n->izq = NULL;
    n->e = malloc(sizeof(entry));
    entry_new(n->e, key, val, valsize);
    n->altura = 1;
    return n;
  }

  // Estoy en un nodo que ya tiene elemento
  int cmp = strcmp(key, nodo->e->key);

  if (cmp < 0) nodo->izq = _avl_insertar(nodo->izq, key, val, valsize);
  else if(cmp > 0) nodo->der = _avl_insertar(nodo->der, key, val, valsize);

  // finalmente balanceamos el AVL
  return _balancear(nodo);
}

void avl_insertar(avl* a, const char *key, void *val, int valsize) {
  a->raiz = _avl_insertar(a->raiz, key, val, valsize);
}

/*
  =================== FIN Inserción
*/


// FREE AVL
static void _free_avl(struct avl_nodo *nodo){
  if(nodo == NULL) return;

  _free_avl(nodo->izq);
  _free_avl(nodo->der);

  free_entry(nodo->e);
  free(nodo->e);
  free(nodo);
}

void avl_dispose(avl *a){
  _free_avl(a->raiz);
  a->raiz = NULL;
}
