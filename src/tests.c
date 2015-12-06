#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vanemdeboas.h"
#include "abb.h"
#include "avl.h"
#include "splaytree.h"
#include "cadenas.h"

#define ELIMINAR 2

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

void test_abb(int eliminar, struct cadena_struct *cs){
  abb a;
  abb_new(&a);

  printf("Insertar %i elementos en un ABB\n", cs->num_elems);
  
  int k;
  for(k=0; k<cs->num_elems; k++){
    char *cadena = get_cadena(cs, k);
    abb_insertar(&a, cadena, cadena, strlen(cadena) + 1);
  }

  printf("Buscar %i elementos en un ABB\n", cs->num_elems);
  int encontrados = 0;
  for (k=0; k<cs->num_elems; k++) {
    char *found = (char*)abb_buscar(&a, get_cadena(cs, k));
    if(found == NULL){
      printf("Warning: No se encontró %s\n", get_cadena(cs, k));
      continue;
    }
    encontrados++;
  }

  printf("Encontrados %i elementos\n", encontrados);

  printf("\nEliminar %i cadenas\n", eliminar);
  for (k=0; k<eliminar; k++) {
    char *cadena = get_cadena(cs, k);
    abb_eliminar(&a, cadena);
  }
  printf("Se eliminaron %i cadenas\n", eliminar);
  puts("Volver a buscar todas las cadenas");

  encontrados = 0;
  for (k=0; k<cs->num_elems; k++) {
    char *cadena = get_cadena(cs, k);
    void *found = abb_buscar(&a, cadena);

    if(found != NULL){
      encontrados++;
      if (k<eliminar) {
        printf("Warning: No se debería haber encontrado %s", (char*)found);
      }
    } else {
      if(k>=eliminar){
        printf("Warning: No se encontró %s\n", get_cadena(cs, k));
      }
    }
  }
  printf("Se espera encontrar %i cadenas\n", (cs->num_elems - eliminar));
  printf("Encontradas %i\n", encontrados);

  abb_dispose(&a);
}


void test_avl(int eliminar, struct cadena_struct *cs){
  avl a;
  avl_new(&a);

  printf("Insertar %i elementos en un AVL\n", cs->num_elems);
  
  int k;
  for(k=0; k<cs->num_elems; k++){
    char *cadena = get_cadena(cs, k);
    avl_insertar(&a, cadena, cadena, strlen(cadena) + 1);
  }

  printf("Buscar %i elementos en un AVL\n", cs->num_elems);
  int encontrados = 0;
  for (k=0; k<cs->num_elems; k++) {
    char *found = (char*)avl_buscar(&a, get_cadena(cs, k));
    if(found == NULL){
      printf("Warning: No se encontró %s\n", get_cadena(cs, k));
      continue;
    }
    encontrados++;
  }

  printf("Encontrados %i elementos\n", encontrados);

  printf("\nEliminar %i cadenas\n", eliminar);
  for (k=0; k<eliminar; k++) {
    char *cadena = get_cadena(cs, k);
    avl_eliminar(&a, cadena);
  }
  printf("Se eliminaron %i cadenas\n", eliminar);
  puts("Volver a buscar todas las cadenas");

  encontrados = 0;
  for (k=0; k<cs->num_elems; k++) {
    char *cadena = get_cadena(cs, k);
    void *found = avl_buscar(&a, cadena);

    if(found != NULL){
      encontrados++;
      if (k<eliminar) {
        printf("Warning: No se debería haber encontrado %s", (char*)found);
      }
    } else {
      if(k>=eliminar){
        printf("Warning: No se encontró %s\n", get_cadena(cs, k));
      }
    }
  }
  printf("Se espera encontrar %i cadenas\n", (cs->num_elems - eliminar));
  printf("Encontradas %i\n", encontrados);

  avl_dispose(&a);
}

void test_splay(int eliminar, struct cadena_struct *cs){
  splaytree s;
  splaytree_new(&s);

  printf("Insertar %i elementos en un Splay Tree\n", cs->num_elems);
  
  int k;
  for(k=0; k<cs->num_elems; k++){
    char *cadena = get_cadena(cs, k);
    splaytree_insertar(&s, cadena, cadena, strlen(cadena) + 1);
  }

  printf("Buscar %i elementos en un Splay Tree\n", cs->num_elems);
  int encontrados = 0;
  for (k=0; k<cs->num_elems; k++) {
    char *found = (char*)splaytree_buscar(&s, get_cadena(cs, k));
    if(found == NULL){
      printf("Warning: No se encontró %s\n", get_cadena(cs, k));
      continue;
    }
    encontrados++;
  }

  printf("Encontrados %i elementos\n", encontrados);
  
  printf("\nEliminar %i cadenas\n", eliminar);
  for (k=0; k<eliminar; k++) {
    char *cadena = get_cadena(cs, k);
    splaytree_eliminar(&s, cadena);
  }
  printf("Se eliminaron %i cadenas\n", eliminar);
  puts("Volver a buscar todas las cadenas");

  encontrados = 0;
  for (k=0; k<cs->num_elems; k++) {
    char *cadena = get_cadena(cs, k);
    void *found = splaytree_buscar(&s, cadena);

    if(found != NULL){
      encontrados++;
      if (k<eliminar) {
        printf("Warning: No se debería haber encontrado %s", (char*)found);
      }
    } else {
      if(k>=eliminar){
        printf("Warning: No se encontró %s\n", get_cadena(cs, k));
      }
    }
  }
  printf("Se espera encontrar %i cadenas\n", (cs->num_elems - eliminar));
  printf("Encontradas %i\n", encontrados);

  splaytree_dispose(&s);
}

int main(int argc, char *argv[]){
  srand48(0);
  if(argc > 1){
    int total = atoi(argv[1]);

    struct cadena_struct cs;
    crear_cadenas(&cs, total);
    eliminar_duplicados(&cs);

    puts("================================Iniciando test ABB");
    if(argc > 2){
      test_abb(atoi(argv[2]), &cs);
    } else test_abb(10, &cs);
    puts("================================Terminar test ABB");

    puts("");
    puts("================================Iniciando test ABB");
    if(argc > 2){
      test_avl(atoi(argv[2]), &cs);
    } else test_avl(10, &cs);
    puts("================================Terminar test ABB");
    
    puts("");
    puts("================================Iniciar test Splay Tree");
    if(argc > 2){
      test_splay(atoi(argv[2]), &cs);
    } else test_splay(10, &cs);
    puts("================================Terminar Test Splay Tree");

    dispose_cadenas(&cs);

    return 0;
  }

  abb_manual();
  
  return 0;
}
