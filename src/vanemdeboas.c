#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "vanemdeboas.h"

// sólo aceptamos strings de largo 15
static unsigned int _hashstring(const char *key){
  if(strlen(key)>0){
    unsigned int n;
    switch(key[0]){
      case 'G':
        n=0;
        break;
      case 'C':
        n=1;
        break;
      case 'A':
        n=2;
        break;
      case 'T':
        n=3;
        break;
    }
    n=n<<(2*strlen(key+1));
    return n + _hashstring(key+1);
  }
  else
    return 0;
}

// retorna los bits más la izquierda de k de x (ie: los más significativos)
static unsigned int _highbits(unsigned int x, unsigned int k){
  //return x>>k;
  return (int)(x/(int)sqrt(k));
}

// retorna los bits más a la derecha de k de x (ie: los menos significativos)
static unsigned int _lowbits(unsigned int x, unsigned int k){
  //return x & ((1<<k) - 1);
  return x % (int)sqrt(k);
}


void vanemdeboas_new(vanemdeboas *veb){
  veb->veb = NULL;
}

static struct vebtree *_veb_insertar(struct vebtree *v, unsigned int key, void *val, int valsize, unsigned int usize){

  // este corresponde a insertar en un veb con size 0
  if(v == NULL){
    v = malloc(sizeof(struct vebtree));
    v->min = v->max = key;
    v->wordsize = usize;

    v->top = NULL;
    v->bottom = (struct vebtree**)calloc(sqrt(usize), sizeof(struct vebtree*));

    return v;
  }

  // el veb tiene elementos
  unsigned int insert = key;
  if(key < v->min){
    insert = v->min;
    v->min = key;
  }

  if(key > v->max) v->max = key;

  if(v->bottom[_highbits(insert, usize)] == NULL)
    v->top = _veb_insertar(v->top, _highbits(insert, v->wordsize), val, valsize, sqrt(v->wordsize));

  v->bottom[_highbits(insert, v->wordsize)] = _veb_insertar(v->bottom[_highbits(insert, v->wordsize)], _lowbits(insert, v->wordsize), val, valsize, sqrt(v->wordsize));

  return v;
}

void vanemdeboas_insertar(vanemdeboas *veb, const char *key, void *val, int valsize){
  veb->veb = _veb_insertar(veb->veb, _hashstring(key), val, valsize, VANEMDEBOAS_UNIVERSO);
}
