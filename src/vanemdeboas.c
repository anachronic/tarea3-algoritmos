#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// retorna los k bits más significativos de x
static unsigned int _highbits(unsigned int x, unsigned int k){
  return x>>k;
}

static unsigned int _lowbits(unsigned int x, unsigned int k){
  return 0; // pendiente
}
