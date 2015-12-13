#include <stdio.h>
#include <sys/time.h>


#include "cadenas.h"
#include "abb.h"
#include "avl.h"
#include "splaytree.h"
//#include "vanemdeboas.h"


static double elapsed_time(struct timeval *a, struct timeval *b){
  return
    (double)(a->tv_sec + (double)a->tv_usec/1000000) -
    (double)(b->tv_sec + (double)b->tv_usec/1000000);
}

int main(int argc, char *argv[])
{
  
  return 0;
}

