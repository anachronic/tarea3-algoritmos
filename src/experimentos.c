#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
  fprintf(f, "#t_err: Error en la medición del tiempo\n");
  fprintf(f, "#o_err: Error en la medición de la ocupación de la estructura\n");

  fprintf(f, "#n\tt\tsize\tt_err\to_err\n");
}

static long double elapsed_time(struct timeval *a, struct timeval *b){
  return
    (long double)(a->tv_sec + (long double)a->tv_usec/1000000) -
    (long double)(b->tv_sec + (long double)b->tv_usec/1000000);
}


// tiempos y ocupacion deben poder aguantar la cantidad de elementos en cs.
// en particular deben ser de tamaño 23 para el experimento default.
static void abb_run_once(struct cadena_struct *cs, long double *tiempos, unsigned long long *ocupacion){
  struct timeval inicio;
  struct timeval fin;
  abb a;
  abb_new(&a);

  int k;
  int potencia = 0;

  long double sumatiempo = 0.0;

  gettimeofday(&inicio, NULL);
  for (k=0; k<cs->num_elems; k++) {
    char *lacadena = get_cadena(cs, k);
    abb_insertar(&a, lacadena, lacadena, TAMANO_CADENA+1);

    if(k+1 == 1<<potencia){
      // por ahora reportamos resultados en pantallax.
      gettimeofday(&fin, NULL);
      // un hack para un comportamiento molesto... el primer elemento asumimos que se inserta en tiempo 0
      if(k==0) tiempos[potencia] = 0.0;
      else {
	sumatiempo += elapsed_time(&fin, &inicio);
	tiempos[potencia] = sumatiempo;
      }
      
      ocupacion[potencia] = abb_espacio(&a);
      potencia++;
      gettimeofday(&inicio, NULL);
    }
  }

  printf("ocupacion final en bytes %llu\n", ocupacion[TAMANO_EXPERIMENTO-1]);

  abb_dispose(&a);
}

int check_errores(long double *tiempos,
		  long double *ocupacion,
		  long double *errores_tiempo,
		  long double *errores_ocupacion,
		  int n,
		  double max_error){
  int i;

  int tiempos_ok = errores_tiempo[0] < max_error * tiempos[0];
  int ocup_ok = errores_ocupacion[0] < max_error * ocupacion[0];

  // sólo chequearemos desde i=10 hasta n.
  
  for (i = 10; i<n; i++) {
    if(!tiempos_ok || !ocup_ok) return 0;

    tiempos_ok = tiempos_ok && errores_tiempo[i] < max_error * tiempos[i];
    ocup_ok = ocup_ok && errores_ocupacion[i] < max_error * ocupacion[i];
  }

  return tiempos_ok && ocup_ok;
}


void experimento_abb(const char *espacio, const char *tipoinput){
  char filename[256];

  sprintf(filename, "abb_%s_%s.dat", espacio, tipoinput);
  FILE *fp = fopen(filename, "w");

  _printheader(fp, filename);

  // variables estadísticas
  // el sufijo t es de tiempo, o es para ocupación
  // stdev_mu es la desviación estandar del promedio
  long double tiempos[TAMANO_EXPERIMENTO];
  long double tiempos2[TAMANO_EXPERIMENTO];
  long double promedio_t[TAMANO_EXPERIMENTO];

  long double tiempos_once[TAMANO_EXPERIMENTO];
  unsigned long long ocupacion_once[TAMANO_EXPERIMENTO];
  
  unsigned long long ocupacion[TAMANO_EXPERIMENTO];
  long double promedio_o[TAMANO_EXPERIMENTO];

  long double stdev_o[TAMANO_EXPERIMENTO];
  long double stdev_t[TAMANO_EXPERIMENTO];

  long double stdev_mu_o[TAMANO_EXPERIMENTO];
  long double stdev_mu_t[TAMANO_EXPERIMENTO];
  
  struct stdev_estadistica se[TAMANO_EXPERIMENTO];

  int MIN_ITERACIONES = 10;
  int iteraciones = 0;
  int k;

  for (k=0; k < TAMANO_EXPERIMENTO; k++) {
    tiempos_once[k] = 0.0;
    ocupacion_once[k] = 0;
    tiempos[k] = 0.0;
    tiempos2[k] = 0.0;
    ocupacion[k] = 0;
    promedio_t[k] = 0.0;
    promedio_o[k] = 0.0;
    stdev_o[k] = 0.0;
    stdev_t[k] = 0.0;
    stdev_mu_o[k] = 0.0;
    stdev_mu_t[k] = 0.0;
    stdev_new(&se[k]);
  }


  while(1){
    struct cadena_struct cs;
    crear_cadenas(&cs, TOTAL_CADENAS);
    if(strcmp(tipoinput, "degenerado") == 0) ordenar_cadenas(&cs);
    
    abb_run_once(&cs, tiempos_once, ocupacion_once);

    for (k=0; k < TAMANO_EXPERIMENTO; k++) {
      tiempos[k] += tiempos_once[k];
      tiempos2[k] += tiempos_once[k] * tiempos_once[k];
      ocupacion[k] += ocupacion_once[k];
      stdev_add(&se[k], ocupacion_once[k]);
    }
    
    iteraciones++;

    if(iteraciones < 2){
      dispose_cadenas(&cs);
      continue;
    }

    int j;
    for (k=0; k < TAMANO_EXPERIMENTO; k++) {
      promedio_t[k] = tiempos[k]/iteraciones;
      promedio_o[k] = (long double)ocupacion[k]/(1.0*iteraciones);

      stdev_t[k] = sqrtl((tiempos2[k] - iteraciones*promedio_t[k]*promedio_t[k])/((double)(iteraciones - 1)));

      stdev_o[k] = 0.0;
      for (j=0; j < se[k].n; j++) {
	long double dif = se[k].valores[j] - promedio_o[k];
	stdev_o[k] += dif*dif/(iteraciones-1);
      }
      stdev_o[k] = sqrtl(stdev_o[k]);
      
      stdev_mu_t[k] = stdev_t[k]/sqrtl(iteraciones*1.0);
      stdev_mu_o[k] = stdev_o[k]/sqrtl(iteraciones*1.0);
    }

    dispose_cadenas(&cs);

    if(iteraciones > MIN_ITERACIONES && check_errores(promedio_t, promedio_o, stdev_mu_t, stdev_mu_o, TAMANO_EXPERIMENTO, 0.05)){
      puts("Experimento completado.");
      break;
    }
  }

  for (k=0; k < TAMANO_EXPERIMENTO; k++) {
    fprintf(fp, "%i\t%.8Lf\t%.2Lf\t%.8Lf\t%.8Lf\n", 1<<k, promedio_t[k], promedio_o[k], stdev_mu_t[k], stdev_mu_o[k]);
  }


  for (k=0; k < TAMANO_EXPERIMENTO; k++) {
    stdev_free(&se[k]);
  }

  fclose(fp);
}


// ---------------- Funciones para el cómputo de desviación estándar
void stdev_new(struct stdev_estadistica *se){
  se->n = 0;
  se->valores = malloc(sizeof(unsigned long long) * STDEV_PREALLOC);
  se->allocLen = STDEV_PREALLOC;
}

void stdev_add(struct stdev_estadistica *se, unsigned long long val){
  if(se->n == se->allocLen){
    se->valores = realloc(se->valores, 2*se->allocLen * sizeof(unsigned long long));
    if(se->valores == NULL){
      fprintf(stderr, "Error: No queda memoria para las estadísticas. gbye\n");
      exit(-1);
    }
    se->allocLen = 2 * se->allocLen;
  }

  se->valores[se->n] = val;
  se->n++;
}

void stdev_free(struct stdev_estadistica *se){
  free(se->valores);
}
