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
  else {
    // cmp == 0
    // el elemento ya está: lo reemplazo por lo nuevo y no balanceo.
    entry_replace_val(nodo->e, val, valsize);
    return nodo;
  }

  // finalmente balanceamos el AVL
  return _balancear(nodo);
}

void avl_insertar(avl* a, const char *key, void *val, int valsize) {
  a->raiz = _avl_insertar(a->raiz, key, val, valsize);
}

/*
  =================== FIN Inserción
  =================== Inicio Eliminación
  La eliminación es bastante parecida a la del abb (ver abb.c, que tiene más comentarios)
  Lo único que cambia es que cada llamada recursiva retorna balancear del nodo donde se borró.
*/
// Elimina el nodo y retorna el nuevo árbol
static struct avl_nodo *_avl_eliminar_nodo(struct avl_nodo *nodo){
  struct avl_nodo *izquierdo;
  struct avl_nodo *derecho;

  if(nodo == NULL) return NULL;

  izquierdo = nodo->izq;
  derecho = nodo->der;

  // elimino la entry
  if(nodo->e != NULL){
    free_entry(nodo->e);
    free(nodo->e);
  }

  free(nodo);

  if(izquierdo != NULL && derecho == NULL) return izquierdo;
  else if (izquierdo == NULL && derecho != NULL) return derecho;

  // El nodo es Hoja.
  return NULL;
}

static struct avl_nodo *_rebalancear(struct avl_nodo *nodo){
  if(nodo == NULL) return NULL;

  nodo->izq = _rebalancear(nodo->izq);
  return _balancear(nodo);
}

static struct avl_nodo *_avl_borrow(struct avl_nodo *nodo, entry *e){
  if (nodo->izq != NULL){
    nodo->izq = _avl_borrow(nodo->izq, e);
    return nodo;
  }

  // no hay hijos izquierdos
  entry_new(e, nodo->e->key, nodo->e->val, nodo->e->val_size);
  return _avl_eliminar_nodo(nodo);
}

// igual a eliminar en ABB. Salvo que retornamos el nodo balanceado.
static struct avl_nodo *_avl_eliminar(struct avl_nodo *nodo, const char *key){
  if(nodo == NULL) return NULL;

  int cmp = strcmp(key, nodo->e->key);

  if(cmp > 0)
    nodo->der = _avl_eliminar(nodo->der, key);
  else if(cmp < 0)
    nodo->izq = _avl_eliminar(nodo->izq, key);
  else {
    // pillé la key en este nodo
    free_entry(nodo->e);
    free(nodo->e);

    // si el nodo tiene un solo hijo lo retorno.
    // Note que al borrar un nodo con hijos <= 1
    // el padre debe BALANCEAR (en contraposición con rebalancear)
    // el nodo.
    if(nodo->der == NULL){
      struct avl_nodo *izquierdo = nodo->izq;
      free(nodo);
      return izquierdo;
    }

    nodo->e = malloc(sizeof(entry));

    nodo->der = _avl_borrow(nodo->der, nodo->e);

    // al hacer brrow hay que rebalancear
    // NOTA: _balancear es O(1) y _rebalancear es O(h) con h: altura del nodo.
    nodo->der = _rebalancear(nodo->der);
  }

  // retorno el arbol rebalanceado
  return _balancear(nodo);
}

void avl_eliminar(avl *a, const char *key){
  a->raiz = _avl_eliminar(a->raiz, key);
}


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
