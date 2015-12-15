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
  fprintf(f, "#elementos: Elementos en la estructura\n");
  fprintf(f, "#tiempo/ocup: Tiempo u ocupación dependiendo de la medición.\n");
  fprintf(f, "#error: Error en la medición del parámetro\n");
  fprintf(f, "#elementos\ttiempo/ocup\terror\n");
}

static long double elapsed_time(struct timeval *a, struct timeval *b){
  return
    (long double)(a->tv_sec + (long double)a->tv_usec/1000000) -
    (long double)(b->tv_sec + (long double)b->tv_usec/1000000);
}

static long intrand(long max){
  return (long)(drand48() * max);
}


// tiempos y ocupacion deben poder aguantar la cantidad de elementos en cs.
// en particular deben ser de tamaño 23 para el experimento default.
static void _run_once(const char *estructura,
		      struct cadena_struct *cs,
		      long double *tiempos_insercion,
		      unsigned long long *ocupacion,
		      long double *tiempos_busqueda_normal,
		      long double *tiempos_busqueda_cargados,
		      long double *tiempos_eliminacion
		      )
{
  struct timeval inicio;
  struct timeval fin;
  abb abb;
  avl avl;
  splaytree stree;

  if(strcmp(estructura, "abb") == 0) abb_new(&abb);
  if(strcmp(estructura, "avl") == 0) avl_new(&avl);
  if(strcmp(estructura, "splaytree") == 0) splaytree_new(&stree);

  int k;
  int potencia = 0;

  long double sumatiempo = 0.0;

  gettimeofday(&inicio, NULL);
  for (k=0; k<cs->num_elems; k++) {
    char *lacadena = get_cadena(cs, k);
    if(strcmp(estructura, "abb") == 0) abb_insertar(&abb, lacadena, lacadena, TAMANO_CADENA+1);
    if(strcmp(estructura, "avl") == 0) avl_insertar(&avl, lacadena, lacadena, TAMANO_CADENA+1);
    if(strcmp(estructura, "splaytree") == 0) splaytree_insertar(&stree, lacadena, lacadena, TAMANO_CADENA+1);

    if(k+1 == 1<<potencia){
      // por ahora reportamos resultados en pantallax.
      gettimeofday(&fin, NULL);
      // un hack para un comportamiento molesto... el primer elemento asumimos que se inserta en tiempo 0
      if(k==0) tiempos_insercion[potencia] = 0.0;
      else {
	sumatiempo += elapsed_time(&fin, &inicio);
	tiempos_insercion[potencia] = sumatiempo;
      }
      
      if(strcmp(estructura, "abb") == 0) ocupacion[potencia] = abb_espacio(&abb);
      if(strcmp(estructura, "avl") == 0) ocupacion[potencia] = avl_espacio(&avl);
      if(strcmp(estructura, "splaytree") == 0) ocupacion[potencia] = splaytree_espacio(&stree);
      potencia++;
      gettimeofday(&inicio, NULL);
    }
  }

  // terminamos las inserciones. Lo buscamos todo
  sumatiempo = 0.0;
  potencia = 0;
  gettimeofday(&inicio, NULL);
  
  for (k=0; k < cs->num_elems; k++) {
    char *lacadena = get_cadena(cs, k);
    if(strcmp(estructura, "abb") == 0) abb_buscar(&abb, lacadena);
    if(strcmp(estructura, "avl") == 0) avl_buscar(&avl, lacadena);
    if(strcmp(estructura, "splaytree") == 0) splaytree_buscar(&stree, lacadena);

    if(k+1 == 1<<potencia){
      gettimeofday(&fin, NULL);
      sumatiempo += elapsed_time(&fin, &inicio);
      tiempos_busqueda_normal[potencia] = sumatiempo;
      potencia++;
      gettimeofday(&inicio, NULL);
    }
  }
  
  // fin búsquedas
  sumatiempo = 0.0;
  potencia = 0;
  gettimeofday(&inicio, NULL);

  for (k=0; k < BUSCAR_CADENAS_RAND; k++) {
    int indicerand = 0;
    int undecimo = (int)cs->num_elems/10;
    if(drand48() < 0.9) indicerand = (int)intrand(undecimo);
    else indicerand = undecimo + intrand(cs->num_elems - undecimo - 1);

    char *lacadena = get_cadena(cs, indicerand);

    if(strcmp(estructura, "abb") == 0) abb_buscar(&abb, lacadena);
    if(strcmp(estructura, "avl") == 0) avl_buscar(&avl, lacadena);
    if(strcmp(estructura, "splaytree") == 0) splaytree_buscar(&stree, lacadena);

    if(k+1 == 1<<potencia){
      gettimeofday(&fin, NULL);
      sumatiempo += elapsed_time(&fin, &inicio);
      tiempos_busqueda_cargados[potencia] = sumatiempo;
      potencia++;
      gettimeofday(&inicio, NULL);
    }
  }

  sumatiempo = 0.0;
  potencia = 0;
  gettimeofday(&inicio, NULL);

  for (k=0; k < cs->num_elems; k++) {
    char *lacadena = get_cadena(cs, k);

    if(strcmp(estructura, "abb") == 0) abb_eliminar(&abb, lacadena);
    if(strcmp(estructura, "avl") == 0) avl_eliminar(&avl, lacadena);
    if(strcmp(estructura, "splaytree") == 0) splaytree_eliminar(&stree, lacadena);

    if (k+1 == 1<<potencia) {
      gettimeofday(&fin, NULL);
      sumatiempo += elapsed_time(&fin, &inicio);
      tiempos_eliminacion[potencia] = sumatiempo;
      potencia++;
      gettimeofday(&inicio, NULL);
    }
  }
  
  
  if(strcmp(estructura, "abb") == 0) abb_dispose(&abb);
  if(strcmp(estructura, "avl") == 0) avl_dispose(&avl);
  if(strcmp(estructura, "splaytree") == 0) splaytree_dispose(&stree);
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


void experimento(const char *estructura, const char *espacio, const char *tipoinput){
  char filename[256];

  sprintf(filename, "%s_%s_%s_insercion.dat", estructura, espacio, tipoinput);
  FILE *fp_in = fopen(filename, "w");
  _printheader(fp_in, filename);
  
  sprintf(filename, "%s_%s_%s_busquedanormal.dat", estructura, espacio, tipoinput);
  FILE *fp_bn = fopen(filename, "w");
  _printheader(fp_bn, filename);
  
  sprintf(filename, "%s_%s_%s_busquedacargada.dat", estructura, espacio, tipoinput);
  FILE *fp_bc = fopen(filename, "w");
  _printheader(fp_bc, filename);
  
  sprintf(filename, "%s_%s_%s_eliminacion.dat", estructura, espacio, tipoinput);
  FILE *fp_el = fopen(filename, "w");
  _printheader(fp_el, filename);
  
  sprintf(filename, "%s_%s_%s_ocupacion.dat", estructura, espacio, tipoinput);
  FILE *fp_oc = fopen(filename, "w");
  _printheader(fp_oc, filename);


  // variables estadísticas
  // el sufijo t es de tiempo, o es para ocupación
  // stdev_mu es la desviación estandar del promedio

  // holy crap.
  long double tiempos_insercion[TAMANO_EXPERIMENTO];
  long double tiempos_busqueda_normal[TAMANO_EXPERIMENTO];
  long double tiempos_busqueda_cargados[TAMANO_EXPERIMENTO];
  long double tiempos_eliminacion[TAMANO_EXPERIMENTO];
  long double tiempos2[TAMANO_EXPERIMENTO];
  long double tiempos_busqueda_n2[TAMANO_EXPERIMENTO];
  long double tiempos_busqueda_c2[TAMANO_EXPERIMENTO];
  long double tiempos_eliminacion2[TAMANO_EXPERIMENTO];
  
  long double promedio_t_insercion[TAMANO_EXPERIMENTO];
  long double promedio_t_busqueda_n[TAMANO_EXPERIMENTO];
  long double promedio_t_busqueda_c[TAMANO_EXPERIMENTO];
  long double promedio_t_eliminacion[TAMANO_EXPERIMENTO];

  long double tiempos_once[TAMANO_EXPERIMENTO];
  long double tiempos_busquedanormal_once[TAMANO_EXPERIMENTO];
  long double tiempos_busquedacargada_once[TAMANO_EXPERIMENTO];
  long double tiempos_eliminacion_once[TAMANO_EXPERIMENTO];
  unsigned long long ocupacion_once[TAMANO_EXPERIMENTO];
  
  unsigned long long ocupacion[TAMANO_EXPERIMENTO];
  long double promedio_o[TAMANO_EXPERIMENTO];

  long double stdev_o[TAMANO_EXPERIMENTO];
  long double stdev_t[TAMANO_EXPERIMENTO];
  long double stdev_t_busqueda_n[TAMANO_EXPERIMENTO];
  long double stdev_t_busqueda_c[TAMANO_EXPERIMENTO];
  long double stdev_t_eliminacion[TAMANO_EXPERIMENTO];

  long double stdev_mu_o[TAMANO_EXPERIMENTO];
  long double stdev_mu_t[TAMANO_EXPERIMENTO];
  long double stdev_mu_t_busqueda_n[TAMANO_EXPERIMENTO];
  long double stdev_mu_t_busqueda_c[TAMANO_EXPERIMENTO];
  long double stdev_mu_t_eliminacion[TAMANO_EXPERIMENTO];
  
  struct stdev_estadistica se[TAMANO_EXPERIMENTO];

  int MIN_ITERACIONES = 10;
  int MAX_ITERACIONES = 50;
  int iteraciones = 0;
  int k;

  for (k=0; k < TAMANO_EXPERIMENTO; k++) {
    tiempos_once[k] = 0.0;
    tiempos_busquedanormal_once[k] = 0.0;
    tiempos_busquedacargada_once[k] = 0.0;
    tiempos_eliminacion_once[k] = 0.0;
    ocupacion_once[k] = 0;
      
    tiempos_insercion[k] = 0.0;
    tiempos_busqueda_normal[k] = 0.0;
    tiempos_busqueda_cargados[k] = 0.0;
    tiempos_eliminacion[k] = 0.0;
    ocupacion[k] = 0;
      
    tiempos2[k] = 0.0;
    tiempos_busqueda_n2[k] = 0.0;
    tiempos_busqueda_c2[k] = 0.0;
    tiempos_eliminacion2[k] = 0.0;
      
    promedio_t_insercion[k] = 0.0;
    promedio_t_busqueda_n[k] = 0.0;
    promedio_t_busqueda_c[k] = 0.0;
    promedio_t_eliminacion[k] = 0.0;
    promedio_o[k] = 0.0;
    
    stdev_o[k] = 0.0;
    stdev_t[k] = 0.0;
    stdev_t_busqueda_n[k] = 0.0;
    stdev_t_busqueda_c[k] = 0.0;
    stdev_t_eliminacion[k] = 0.0;
    
    stdev_mu_o[k] = 0.0;
    stdev_mu_t[k] = 0.0;
    stdev_mu_t_busqueda_n[k] = 0.0;
    stdev_mu_t_busqueda_c[k] = 0.0;
    stdev_mu_t_eliminacion[k] = 0.0;
    stdev_new(&se[k]);
  }

  while(1){
    struct cadena_struct cs;
    if(strcmp(espacio, "real") == 0){
      // obtener cadenas desde el archivo
      crear_cadenas_file(&cs, TOTAL_CADENAS);
    } else {
      // random.
      crear_cadenas(&cs, TOTAL_CADENAS);
    }
    
    if(strcmp(tipoinput, "degenerado") == 0){
      ordenar_cadenas(&cs);
    }
    
    _run_once(estructura,
	      &cs,
	      tiempos_once,
	      ocupacion_once,
	      tiempos_busquedanormal_once,
	      tiempos_busquedacargada_once,
	      tiempos_eliminacion_once
	      );

    for (k=0; k < TAMANO_EXPERIMENTO; k++) {
      tiempos_insercion[k] += tiempos_once[k];
      tiempos_busqueda_normal[k] += tiempos_busquedanormal_once[k];
      tiempos_eliminacion[k] += tiempos_once[k];
      
      tiempos2[k] += tiempos_once[k] * tiempos_once[k];
      tiempos_busqueda_n2[k] += tiempos_busquedanormal_once[k] * tiempos_busquedanormal_once[k];
      tiempos_eliminacion2[k] += tiempos_eliminacion_once[k] * tiempos_eliminacion_once[k];
      
      if(k < CADENAS_RAND){
	tiempos_busqueda_cargados[k] += tiempos_busquedacargada_once[k];
	tiempos_busqueda_c2[k] += tiempos_busquedacargada_once[k] * tiempos_busquedacargada_once[k];
      }
      
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
      promedio_t_insercion[k] = tiempos_insercion[k]/iteraciones;
      promedio_t_busqueda_n[k] = tiempos_busqueda_normal[k]/iteraciones;
      promedio_t_eliminacion[k] = tiempos_eliminacion[k]/iteraciones;
      promedio_o[k] = (long double)ocupacion[k]/(1.0*iteraciones);

      if(k < CADENAS_RAND){
	promedio_t_busqueda_c[k] = tiempos_busqueda_cargados[k]/iteraciones;
      }

      stdev_t[k] = sqrtl((tiempos2[k] - iteraciones*promedio_t_insercion[k]*promedio_t_insercion[k])/((double)(iteraciones - 1)));
      stdev_t_busqueda_n[k] = sqrtl((tiempos_busqueda_n2[k] - iteraciones*promedio_t_busqueda_n[k]*promedio_t_busqueda_n[k])/((double)(iteraciones - 1)));
      stdev_t_eliminacion[k] = sqrtl((tiempos_eliminacion2[k] - iteraciones*promedio_t_eliminacion[k]*promedio_t_eliminacion[k])/((double)(iteraciones - 1)));
      if(k < CADENAS_RAND){
	stdev_t_busqueda_c[k] = sqrtl((tiempos_busqueda_c2[k] - iteraciones*promedio_t_busqueda_c[k]*promedio_t_busqueda_c[k])/((double)(iteraciones - 1)));
      }

      stdev_o[k] = 0.0;
      for (j=0; j < se[k].n; j++) {
	long double dif = se[k].valores[j] - promedio_o[k];
	stdev_o[k] += dif*dif/(iteraciones-1);
      }
      stdev_o[k] = sqrtl(stdev_o[k]);
      
      stdev_mu_t[k] = stdev_t[k]/sqrtl(iteraciones*1.0);
      stdev_mu_t_busqueda_n[k] = stdev_t_busqueda_n[k]/sqrtl(iteraciones*1.0);
      stdev_mu_t_eliminacion[k] = stdev_t_eliminacion[k]/sqrtl(iteraciones*1.0);
      stdev_mu_o[k] = stdev_o[k]/sqrtl(iteraciones*1.0);

      if(k < BUSCAR_CADENAS_RAND){
	stdev_mu_t_busqueda_c[k] = stdev_t_busqueda_c[k]/sqrtl(iteraciones*1.0);
      }
    }

    dispose_cadenas(&cs);

    if(iteraciones > MIN_ITERACIONES && check_errores(promedio_t_insercion,
						      promedio_o,
						      stdev_mu_t,
						      stdev_mu_o,
						      TAMANO_EXPERIMENTO, 0.05)){
      puts("Experimento completado.");
      break;
    }

    if(iteraciones > MAX_ITERACIONES){
      puts("Cantidad máxima de iteraciones alcanzada, el error puede ser un poco más alto de lo esperado");
      break;
    }
  }

  for (k=0; k < TAMANO_EXPERIMENTO; k++) {
    fprintf(fp_in, "%i\t%.8Lf\t%.8Lf\n", 1<<k, promedio_t_insercion[k], stdev_mu_t[k]);
    fprintf(fp_bn, "%i\t%.8Lf\t%.8Lf\n", 1<<k, promedio_t_busqueda_n[k], stdev_mu_t_busqueda_n[k]);
    fprintf(fp_el, "%i\t%.8Lf\t%.8Lf\n", 1<<k, promedio_t_eliminacion[k], stdev_mu_t_eliminacion[k]);
    fprintf(fp_oc, "%i\t%.2Lf\t%.8Lf\n", 1<<k, promedio_o[k], stdev_mu_o[k]);

    if(k < CADENAS_RAND)
      fprintf(fp_bc, "%i\t%.8Lf\t%.8Lf\n", 1<<k, promedio_t_busqueda_c[k], stdev_mu_t_busqueda_c[k]);
  }


  for (k=0; k < TAMANO_EXPERIMENTO; k++) {
    stdev_free(&se[k]);
  }

  fclose(fp_in);
  fclose(fp_bc);
  fclose(fp_bn);
  fclose(fp_el);
  fclose(fp_oc);
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
