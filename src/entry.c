#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "entry.h"

entry *duplicar_entry(entry *e){
  entry *newentry = (entry*)malloc(sizeof(entry));
  entry->key = strdup(e->key);
  entry->val = malloc(e->val_size);
  memcpy(entry->val, e->val, e->val_size);

  return newentry;
}

void free_entry(entry *e){
  free(e->val);
  free(e->key);
}

void entry_new(entry *e, char *key, void *val, int size){
  e->key = strdup(key);
  e->val = malloc(size);
  memcpy(e->val, val, size);
}
