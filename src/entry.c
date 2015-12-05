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
