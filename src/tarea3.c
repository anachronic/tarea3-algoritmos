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
  // primero revisamos el tamaño del stack
  struct rlimit rl;
  getrlimit(RLIMIT_STACK, &rl);

  rl.rlim_cur = 600*rl.rlim_cur;
  setrlimit(RLIMIT_STACK, &rl);

  printf("Tamaño máximo actual de stack es: %lu\n", rl.rlim_cur);
  printf("Tamaño máximo global de stack es: %lu\n", rl.rlim_max);
  
  srand48(getpid());
  
  experimento("abb", "sintetico", "random");
  /* experimento_abb("sintetico", "degenerado"); */

  return 0;
}

