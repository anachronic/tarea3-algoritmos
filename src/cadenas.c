#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#include "cadenas.h"

#define TAMANO_CADENA 15

static long intrand(long max){
  return (long)(drand48() * max);
}

char *adn_from_file(const char *file){
  FILE *fp = fopen(file, "rb");

  if(fp == NULL){
    fprintf(stderr, "No se encontró el archivo de DNA. Revise cómo se debiera llamar en cadenas.h");
    exit(-1);
  }

  long filesize;
  fseek(fp, 0, SEEK_END);
  filesize = ftell(fp);

  long patronstart = intrand(filesize - 1 - TAMANO_CADENA);
  fseek(fp, patronstart, SEEK_SET);

  char *buffer = malloc(TAMANO_CADENA + 1);

  fread(buffer, sizeof(char), TAMANO_CADENA, fp);
  buffer[TAMANO_CADENA] = 0;

  fclose(fp);

  return buffer;
}



char base_rand(){
  double r = drand48();

  if(r < 0.25) return 'A';
  else if (r < 0.5) return 'T';
  else if (r < 0.75) return 'G';
  else return 'C';
}

// Para usar esta función se debe PROVEER un array de chars
// de tamaño AL MENOS 16
void cadena_rand(char *buffer){
  int k;

  for(k=0; k<TAMANO_CADENA; k++){
    buffer[k] = base_rand();
  }
  buffer[k] = 0;
}

char *get_random_from_array(char **arr, long size){
  long index;

  index = intrand(size);
  return arr[index];
}

/** Estructura para guardar las cadenas **/
void crear_cadenas(struct cadena_struct *cs, int size){
  cs->total_alloc = size;
  cs->num_elems = size;
  cs->cadenas = (char**)malloc(sizeof(char*)*size);

  int k;
  for(k=0; k<cs->num_elems; k++){
    cs->cadenas[k] = (char*)malloc(TAMANO_CADENA+1);
    cadena_rand(cs->cadenas[k]);
  }
}

char *get_cadena(struct cadena_struct *cs, int k){
  return cs->cadenas[k];
}

void eliminar_cadena(struct cadena_struct *cs, int k){
  if(cs == NULL || cs->cadenas == NULL || cs->cadenas[k] == NULL) return;

  free(cs->cadenas[k]);
  if(cs->num_elems - k - 1 > 0)
    memmove(cs->cadenas + k, cs->cadenas + k + 1, sizeof(char*)*(cs->num_elems - k - 1));
  cs->num_elems--;
}

void dispose_cadenas(struct cadena_struct *cs){
  int k;

  if(cs!=NULL){
    if(cs->cadenas != NULL){
      for(k=0; k<cs->num_elems; k++){
        free(cs->cadenas[k]);
      }
      free(cs->cadenas);
    }
  }
}

void eliminar_duplicados(struct cadena_struct *cs){
  int k = 0;
  int i = 0;

  while(k < cs->num_elems){
    char *esta = get_cadena(cs, k);

    i=k+1;
    while(i < cs->num_elems){
      // si cadenas[i] es igual, la sacamos
      if(strcmp(cs->cadenas[i], esta) == 0){
        free(cs->cadenas[i]);
        cs->num_elems--;
        if(cs->num_elems - i > 0)
          memmove(cs->cadenas + i, cs->cadenas + i + 1, sizeof(char*) * (cs->num_elems - i));

        // no aumentamos i. Disminuyó el tamaño
        continue;
      }
      i++;
    }

    k++;
  }
}

int _cmpstr(const void *a, const void *b){
  const char *aa = *(const char **)a;
  const char *bb = *(const char **)b;
  return strcmp(aa, bb);
}

void ordenar_cadenas(struct cadena_struct *cs){
  qsort(cs->cadenas, cs->num_elems, sizeof(char*), _cmpstr);
}
