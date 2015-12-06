#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "splaytree.h"


void splaytree_new(splaytree *s){
  s->raiz = NULL;
}

/*
  ===== Primero las funciones zig y zag. Al final splay.
*/

// zig: Rotación Horario
// retorna el padre luego de hacer zig sobre nodo
static struct splay_nodo *_zig(struct splay_nodo *nodo){
  struct splay_nodo *nueva_raiz = nodo->izq;

  nodo->izq = nueva_raiz->der;
  nueva_raiz->der = nodo;
  return nueva_raiz;
}

// zag: Rotación Antihorario
// retorna el padre luego de hacer zag sobre nodo
static struct splay_nodo *_zag(struct splay_nodo *nodo){
  struct splay_nodo *nueva_raiz = nodo->der;

  nodo->der = nueva_raiz->izq;
  nueva_raiz->izq = nodo;
  return nueva_raiz;
}

static struct splay_nodo *_splay(struct splay_nodo *nodo, const char *key){
  if(nodo == NULL) return NULL;

  int cmp = strcmp(key, nodo->e->key);

  // está en la raíz. (de este subárbol al menos)
  // no hacemos zig, ni zag ni zigzag ni NADAAAAAAAAAAAAAAAAa
  if(cmp == 0) return nodo;

  if(cmp < 0){
    // sabemos que está en el lado izquierdo
    // no hay elementos a la izquierda? Termino el splay.
    if(nodo->izq == NULL) return nodo;

    int cmp2 = strcmp(key, nodo->izq->e->key);
    // Veo por donde me voy
    if (cmp2 < 0) {
      // El nodo está por la izquierda (del subárbol izquierdo).
      // Dejo el elemento en el hijo izquierdo de mi subárbol izquierdo
      nodo->izq->izq = _splay(nodo->izq->izq, key);

      // Lo quiero dejar en la raíz de mi subárbol izquierdo.
      // Me hago zig sobre mí mismo y el elem queda en la raíz del subárbol izq.
      nodo = _zig(nodo);
    } else if (cmp2 > 0) {
      // Si está por la derecha.. (del subárbol izquierdo)
      // Se trae a la raíz del hijo derecho del subárbol izquierdo de nodo el elemento
      // Cuando nodo->izq->der contiene al elemento (si existe) le hacemos zag para dejarlo
      // en la raíz subárbol izquierdo.
      nodo->izq->der = _splay(nodo->izq->der, key);
      if (nodo->izq->der != NULL)
        nodo->izq = _zag(nodo->izq);
    }

    // ya tengo el elemento en la raíz de mi subárbol izquierdo si existe.
    // si existe, un zig me lo deja en la raíz.
    if (nodo->izq == NULL) return nodo;
    return _zig(nodo);
  }

  // cmp > 0
  if(nodo->der == NULL) return NULL;
  
  int cmp3 = strcmp(key, nodo->der->e->key);
  if(cmp3 > 0){
    // está por la derecha del subárbol derecho
    // primero dejo el elemento en la raíz del hijo derecho de mi subárbol derecho
    nodo->der->der = _splay(nodo->der->der, key);

    // cuando ya está ahí, hago zag sobre mí mismo y lo dejo en la raíz de mi subárbol derecho.
    nodo = _zag(nodo);
  } else if (cmp3 < 0) {
    // Está por la izquierda de mi subárbol derecho.
    // Lo llevo a la raíz del hijo izquierdo de mi subárbol derecho primero
    nodo->der->izq = _splay(nodo->der->izq, key);

    // Lo quiero dejar en la raíz de mi subárbol derecho. Zig sobre nodo->der
    if(nodo->der->izq != NULL)
      nodo->der = _zig(nodo->der);
  }

  // Lo tengo listo. Si existe, está en nodo->der
  // y un zag sobre mí mismo lo deja en la raíz.
  if(nodo->der == NULL) return nodo;
  return _zag(nodo);
}
/*
  ============================== FIN Operaciones intrínsecas del Splay Tree
*/

void *splaytree_buscar(splaytree *s, const char *key){
  // Dejo el elemento en la raíz (si es que está)
  s->raiz = _splay(s->raiz, key);
  if(strcmp(key, s->raiz->e->key) == 0) return s->raiz->e->val;

  // no está...
  return NULL;
}

// Inserta el nodo tal cual en un ABB. Se deja la responsabilidad de hacer splay
// a la función que llama a esta (ie: splaytree_insertar. Note que no está el _)
struct splay_nodo *_splaytree_insertar(struct splay_nodo *nodo, const char *key, void *val, int valsize){
  if(nodo == NULL){
    struct splay_nodo *n = malloc(sizeof(struct splay_nodo));
    n->izq = NULL;
    n->der = NULL;
    n->e = malloc(sizeof(entry));
    entry_new(n->e, key, val, valsize);
    return n;
  }

  int cmp = strcmp(key, nodo->e->key);
  
  // si ya está, la reemplazo.
  if (cmp == 0){
    entry_replace_val(nodo->e, val, valsize);
    return nodo;
  }

  if(cmp < 0) nodo->izq = _splaytree_insertar(nodo->izq, key, val, valsize);
  else nodo->der = _splaytree_insertar(nodo->der, key, val, valsize);

  return nodo;
}

void splaytree_insertar(splaytree *s, const char *key, void *val, int valsize){
  s->raiz = _splaytree_insertar(s->raiz, key, val, valsize);
  s->raiz = _splay(s->raiz, key);
}


void _free_splaytree(struct splay_nodo *nodo){
  if(nodo == NULL) return;

  _free_splaytree(nodo->izq);
  _free_splaytree(nodo->der);

  free_entry(nodo->e);
  free(nodo->e);
  free(nodo);
}

void splaytree_dispose(splaytree *s){
  _free_splaytree(s->raiz);
  s->raiz = NULL;
}
