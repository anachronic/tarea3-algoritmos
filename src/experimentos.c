#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "experimentos.h"
#include "abb.h"
#include "avl.h"
#include "splaytree.h"
#include "cadenas.h"

static void _printheader(FILE *f, const char *estructura){
  fprintf(f, "# Resultados para %s\n", estructura);
  fprintf(f, "#n: Elementos en la estructura\n");
  fprintf(f, "#t: Tiempo acumulado de ejecución\n");
  fprintf(f, "#size: Tamaño en bytes de la estructura\n");
  fprintf(f, "#n\tt\tsize\n");
}

static double elapsed_time(struct timeval *a, struct timeval *b){
  return
    (double)(a->tv_sec + (double)a->tv_usec/1000000) -
    (double)(b->tv_sec + (double)b->tv_usec/1000000);
}


void experimento_abb(const char *espacio){
  char filename[256];
  char tipoinput[256];

  sprintf(tipoinput, "random");

  sprintf(filename, "abb_%s_%s.dat", espacio, tipoinput);
  FILE *fp = fopen(filename, "w");

  _printheader(fp, filename);

  abb a;
  abb_new(&a);

  // generamos una struct de cadenas nuevas aleatorias (este es el primer experimento)
  struct cadena_struct cs;
  crear_cadenas(&cs, TOTAL_CADENAS);

  // ok, ok, esto es terrible y tiene pinta de que va a correr por varios días.
  // 1. medimos tiempo y espacio para cada uno de los 2^k.
  // 2. calculamos el error asociado a todas las mediciones que llevamos
  // 3. si TODOS los errores no son menores a lo esperado, repetir el experimento
  // Esto es un infierno. maybe reducir el espacio a 2^20?

  int k;
  int potencia = 0;
  struct timeval inicio, fin;

  gettimeofday(&inicio, NULL);
  for (k=0; k<cs.num_elems; k++) {
    char *lacadena = get_cadena(&cs, k);
    abb_insertar(&a, lacadena, lacadena, TAMANO_CADENA+1);

    if(k == 1<<potencia){
      // por ahora reportamos resultados en pantallax.
      gettimeofday(&fin, NULL);
      printf("elems: %i\ttiempo: %.8f\tocupacion: %llu\n", k, elapsed_time(&fin, &inicio), abb_espacio(&a));
      potencia++;
    }
  }

  dispose_cadenas(&cs);
  fclose(fp);
}
