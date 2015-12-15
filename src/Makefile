CC=gcc
CFLAGS=-g -Wall
LDFLAGS=-lm
PROGNAME=tarea3
DEPS = avl.h abb.h splaytree.h vanemdeboas.h entry.h cadenas.h experimentos.h
OBJS = avl.o abb.o splaytree.o vanemdeboas.o entry.o cadenas.o experimentos.o

default : $(PROGNAME)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tests.o :
	$(CC) $(CFLAGS) -o tests.o tests.c -c

$(PROGNAME).o :
	$(CC) $(CFLAGS) -o $(PROGNAME).o $(PROGNAME).c -c

tests : $(OBJS) tests.o
	gcc -o tests $^ $(CFLAGS) $(LDFLAGS)

$(PROGNAME) : $(OBJS) tarea3.o
	gcc -o $(PROGNAME) $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf *.o *.dat $(PROGNAME)

debug:
	make clean && make
