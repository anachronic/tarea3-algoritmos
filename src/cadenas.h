#ifndef TAREA2_ALGORITMOS_CADENAS_H
#define TAREA2_ALGORITMOS_CADENAS_H

#define BUSCAR_CADENAS_RAND (1<<16)
#define TOTAL_CADENAS (1<<25)

#define ADN_FILE "texts/dna.200MB"

char base_rand();
void cadena_rand(char *buffer);
char *get_random_from_array(char **arr, long size);

struct cadena_struct{
  int total_alloc;
  int num_elems;
  char **cadenas;
};

void crear_cadenas(struct cadena_struct *cs, int size);
char *get_cadena(struct cadena_struct *cs, int k);
void eliminar_cadena(struct cadena_struct *cs, int k);
void dispose_cadenas(struct cadena_struct *cs);
void eliminar_duplicados(struct cadena_struct *cs);
void ordenar_cadenas(struct cadena_struct *cs);

char *adn_from_file(const char *file);

#endif //TAREA2_ALGORITMOS_CADENAS_H
