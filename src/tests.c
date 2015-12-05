#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vanemdeboas.h"
#include "abb.h"
#include "avl.h"
#include "splaytree.h"
#include "cadenas.h"

#define ELIMINAR 2

void test_abb(int total){
  abb a;
  abb_new(&a);

  struct cadena_struct cs;
  crear_cadenas(&cs, total);

  int k;

  for (k=0; k < cs.num_elems; k++) {
    abb_insertar(&a, get_cadena(&cs, k), "cadena", 7);
  }

  int encontrados = 0;
  for (k=0; k<total; k++) {
    char *found = (char*)abb_buscar(&a, get_cadena(&cs, k));
    if(found == NULL){
      printf("Warning: No se encontró %s\n", get_cadena(&cs, k));
      continue;
    }
    encontrados++;
  }

  // eliminar todo.
  for(k=0; k<total; k++){
    abb_eliminar(&a, get_cadena(&cs, k));
  }

  // test eliminar cadena que no existe
  abb_eliminar(&a, "ansdklmsdk");


  puts("No se debe encontrar ningún elemento");
  // no debo encontrar nada
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

void abb_manual(){
  abb a;
  abb_new(&a);

  puts("Insertar X, W, Y K");
  abb_insertar(&a, "X", "x", 2);
  abb_insertar(&a, "W", "w", 2);
  abb_insertar(&a, "Y", "y", 2);
  abb_insertar(&a, "K", "k", 2);

  puts("Eliminar X");
  abb_eliminar(&a, "X");

  puts("Buscar X W Y K");

  printf("Buscar X: ");
  if(abb_buscar(&a, "X") != NULL) puts("encontrado");
  else puts("no encontrado");

  printf("Buscar Y: ");
  if(abb_buscar(&a, "Y") != NULL) puts("encontrado");
  else puts("no encontrado");

  printf("Buscar W: ");
  if(abb_buscar(&a, "W") != NULL) puts("encontrado");
  else puts("no encontrado");

  printf("Buscar K: ");
  if(abb_buscar(&a, "K") != NULL) puts("encontrado");
  else puts("no encontrado");

  puts("Resultado esperado: No encontrar X. Encontrar Y W K");
  abb_dispose(&a);
}

char* _debug_print_avl(struct avl_nodo *n){
  if(n == NULL) return "";
  char *buffer = malloc(2000);

  sprintf(buffer, "izquierdo={%s}\tnodo={%s}\tderecho={%s}", _debug_print_avl(n->izq), n->e->key, _debug_print_avl(n->der));
  return buffer;
}


void test_avl(int total, int eliminar){
  avl a;
  avl_new(&a);

  struct cadena_struct cs;
  crear_cadenas(&cs, total);

  printf("Insertar %i elementos en un AVL\n", total);
  
  int k;
  for(k=0; k<cs.num_elems; k++){
    char *cadena = get_cadena(&cs, k);
    avl_insertar(&a, cadena, cadena, strlen(cadena) + 1);
  }

  printf("Buscar %i elementos en un AVL\n", cs.num_elems);
  int encontrados = 0;
  for (k=0; k<cs.num_elems; k++) {
    char *found = (char*)avl_buscar(&a, get_cadena(&cs, k));
    if(found == NULL){
      printf("Warning: No se encontró %s\n", get_cadena(&cs, k));
      continue;
    }
    encontrados++;
  }

  printf("Encontrados %i elementos\n", encontrados);

  printf("\nEliminar %i cadenas\n", eliminar);
  for (k=0; k<eliminar; k++) {
    char *cadena = get_cadena(&cs, k);
    avl_eliminar(&a, cadena);
  }
  printf("Se eliminaron %i cadenas\n", eliminar);
  puts("Volver a buscar todas las cadenas");

  encontrados = 0;
  for (k=0; k<cs.num_elems; k++) {
    char *cadena = get_cadena(&cs, k);
    void *found = avl_buscar(&a, cadena);

    if(found != NULL){
      encontrados++;
      if (k<eliminar) {
        printf("Warning: No se debería haber encontrado %s", (char*)found);
      }
    }
  }
  printf("Se espera encontrar %i cadenas\n", (cs.num_elems - eliminar));
  printf("Encontradas %i\n", encontrados);


  dispose_cadenas(&cs);
  avl_dispose(&a);

}

int main(int argc, char *argv[]){
  srand48(0);
  if(argc > 1){
    int total = atoi(argv[1]);

    puts("Iniciando test ABB");
    test_abb(total);
    puts("Terminar test ABB");

    puts("");
    puts("Iniciando test AVL");
    if(argc > 2){
      test_avl(total, atoi(argv[2]));
    } else test_avl(total, 10);
    puts("Terminar test AVL");
    

    return 0;
  }

  abb_manual();
  
  return 0;
}
