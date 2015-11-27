#ifndef TAREA3_ALGORITMOS_ENTRY_H
#define TAREA3_ALGORITMOS_ENTRY_H

typedef struct pair_entry{
  char *key;
  void *val;
  int val_size; // tamaño en bytes de val.
} entry;

entry *duplicar_entry(entry *e);
void free_entry(entry *e);
void entry_new(entry *e, char *key, void *val, int size);

#endif //TAREA3_ALGORITMOS_ENTRY_H
