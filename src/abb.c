#include "abb.h"


void abb_new(abb *a){
  a->arbol = (struct abb_nodo *)malloc(sizeof(struct abb_nodo));
  a->arbol = NULL;
}








static void _free_abb(struct abb_nodo *nodo){
  if(nodo == NULL) return;

  _free_abb(nodo->izq);
  _free_abb(nodo->der);

  if(nodo->izq != NULL) free(nodo->izq);
  if(nodo->der != NULL) free(nodo->der);

  free_entry(nodo->e);
  free(e);
}
