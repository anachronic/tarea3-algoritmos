#include <stdio.h>
#include <sys/time.h>


#include "cadenas.h"
#include "abb.h"
#include "avl.h"
#include "splaytree.h"
//#include "vanemdeboas.h"
#include "experimentos.h"



int main(int argc, char *argv[])
{
  experimento_abb("real", "random");
  experimento_abb("real", "degenerado");


  return 0;
}

