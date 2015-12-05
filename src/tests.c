#include <stdio.h>
#include <stdlib.h>

#include "vanemdeboas.h"
#include "abb.h"
#include "avl.h"
#include "splaytree.h"
#include "cadenas.h"

#define LARGO_TEST 10


void test_abb(int total){
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

  // eliminar todo.
  for(k=0; k<total; k++){
    abb_eliminar(&a, get_cadena(&cs, k));
  }

  // test eliminar cadena que no existe
  abb_eliminar(&a, "ansdklmsdk");


  puts("No se debe encontrar ningún elemento");
  // no debo encontrar nada
  int encontrados = 0;
  for (k=0; k<total; k++) {
    char *found = (char*)abb_buscar(&a, get_cadena(&cs, k));
    if(found != NULL){
      encontrados++;
    }
  }

  printf("Se buscaron %i elementos y se encontraron %i\n", total, encontrados);

  dispose_cadenas(&cs);
  abb_dispose(&a);
}

int main(int argc, char *argv[]){
  int total = atoi(argv[1]);

  test_abb(total);
  
  return 0;
}
