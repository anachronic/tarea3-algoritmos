#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>


#include "cadenas.h"
#include "abb.h"
#include "avl.h"
#include "splaytree.h"
//#include "vanemdeboas.h"
#include "experimentos.h"



int main(int argc, char *argv[])
{

  puts("==========TAREA 3 Diseño y Análisis de Algoritmos==================");
  // primero revisamos el tamaño del stack
  struct rlimit rl;
  getrlimit(RLIMIT_STACK, &rl);

  rl.rlim_cur = 600*rl.rlim_cur;
  setrlimit(RLIMIT_STACK, &rl);

  printf("Tamaño máximo actual de stack es: %lu\n", rl.rlim_cur);
  printf("Tamaño máximo global de stack es: %lu\n", rl.rlim_max);
  
  srand48(getpid());

  puts("Todos los experimentos pueden tardar algunos minutos. Por favor espere...");
  puts("Iniciar experimento ABB - Sintético - Random");
  experimento("abb", "sintetico", "random");
  puts("Finalizar experimento ABB - Sintético - Random");

  puts("Iniciar experimento AVL - Sintético - Random");
  experimento("avl", "sintetico", "random");
  puts("Finalizar experimento AVL - Sintético - Random");
  
  puts("Iniciar experimento Splay Tree - Sintético - Random");
  experimento("splaytree", "sintetico", "random");
  puts("Finalizar experimento Splay Tree - Sintético - Random");
  
  puts("Iniciar experimento AVL - Sintético - Degenerado");
  experimento("avl", "sintetico", "degenerado");
  puts("Finalizar experimento AVL - Sintético - Degenerado");
  
  puts("Iniciar experimento Splay Tree - Sintético - Degenerado");
  experimento("splaytree", "sintetico", "degenerado");
  puts("Finalizar experimento Splay Tree - Sintético - Degenerado");

  puts("Iniciar experimento ABB - Real - Random");
  experimento("abb", "real", "random");
  puts("Finalizar experimento ABB - Real - Random");
  
  puts("Iniciar experimento AVL - Real - Random");
  experimento("avl", "real", "random");
  puts("Finalizar experimento AVL - Real - Random");

  puts("Finalizar experimento Splay Tree - Real - Random");
  experimento("splaytree", "real", "random");
  puts("Finalizar experimento Splay Tree - Real - Random");
  
  puts("Iniciar experimento AVL - Real - Degenerado");
  experimento("avl", "real", "degenerado");
  puts("Finalizar experimento AVL - Real - Degenerado");

  puts("Iniciar experimento Splay Tree - Real - Degenerado");
  experimento("splaytree", "real", "degenerado");
  puts("Finalizar experimento Splay Tree - Real - Degenerado");

  // ABB sintético degenerado se va a la cresta en tiempo....
  /* experimento_abb("sintetico", "degenerado"); */
  
  puts("========= Fin Tarea 3 Diseño y Análisis de Algoritmos");
  return 0;
}

