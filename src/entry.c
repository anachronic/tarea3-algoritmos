#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "entry.h"

entry *duplicar_entry(entry *e){
  entry *newentry = (entry*)malloc(sizeof(entry));
  newentry->key = strdup(e->key);
  newentry->val = malloc(e->val_size);
  memcpy(newentry->val, e->val, e->val_size);

  return newentry;
}

void free_entry(entry *e){
  free(e->val);
  free(e->key);
}

void entry_new(entry *e, const char *key, void *val, int size){
  e->key = strdup(key);
  e->val = malloc(size);
  e->val_size = size;
  memcpy(e->val, val, size);
}

void entry_replace_val(entry *e, void *val, int valsize){
  free(e->val);
  e->val = malloc(valsize);
  e->val_size = valsize;
  memcpy(e->val, val, valsize);
}

unsigned long long entry_size(entry *e){
  if (e == NULL) return 0;

  // la estructura pesa 2 punteros + un entero (ie: sizeof(entry))
  unsigned long long peso = sizeof(entry);

  // y la entry guarda e->val_size bytes además.
  return peso + e->val_size;
}
