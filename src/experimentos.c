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

  // variables propias de la iteración y mediciones
  int k;
  int potencia = 0;
  struct timeval inicio, fin;

  // variables estadísticas
  // el sufijo t es de tiempo, o es para ocupación
  // stdev_mu es la desviación estandar del promedio
  long double suma_t = 0.0;
  unsigned long long suma_o = 0;
  
  long double promedio_t[TAMANO_EXPERIMENTO];
  unsigned long long promedio_o[TAMANO_EXPERIMENTO];

  long double stdev_o[TAMANO_EXPERIMENTO];
  long double stdev_t[TAMANO_EXPERIMENTO];

  long double stdev_mu_o, stdev_mu_t;

  gettimeofday(&inicio, NULL);
  for (k=0; k<cs.num_elems; k++) {
    char *lacadena = get_cadena(&cs, k);
    abb_insertar(&a, lacadena, lacadena, TAMANO_CADENA+1);

    if(k+1 == 1<<potencia){
      // por ahora reportamos resultados en pantallax.
      gettimeofday(&fin, NULL);
      promedio_o[potencia] = abb_espacio(&a);
      promedio_t[potencia] = elapsed_time(&fin, &inicio);
      potencia++;
    }
  }

  for (k=0; k<TAMANO_EXPERIMENTO; k++) {
    printf("elems: %i\ttiempo: %.8Lf\tocupacion: %llu\n", k, promedio_t[k], promedio_o[k]);
  }


  dispose_cadenas(&cs);
  fclose(fp);
}
