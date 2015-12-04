#include <stdio.h>
#include <stdlib.h>

#include "vanemdeboas.h"
#include "abb.h"
#include "avl.h"
#include "splaytree.h"
#include "cadenas.h"

#define LARGO_TEST 10


int main(int argc, char *argv[]){
  int total = atoi(argv[1]);

  abb a;
  abb_new(&a);

  struct cadena_struct cs;
  crear_cadenas(&cs, total);

  int k;

  for (k=0; k < cs.num_elems; k++) {
    abb_insertar(&a, get_cadena(&cs, k), "cadena", 7);
  }

  for (k=0; k<total; k++) {
    char *found = (char*)abb_buscar(&a, get_cadena(&cs, k));
    if(found == NULL){
      printf("Warning: No se encontró %s\n", get_cadena(&cs, k));
      continue;
    }
    printf("%s\n", found);
  }

  dispose_cadenas(&cs);

  
  return 0;
}
