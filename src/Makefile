CC=gcc
CFLAGS=-g -Wall
LDFLAGS=-lm
PROGNAME=tarea3
DEPS = avl.h abb.h splaytree.h vanemdeboas.h entry.h cadenas.h
OBJS = avl.o abb.o splaytree.o vanemdeboas.o entry.o cadenas.o

default : $(PROGNAME)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tests.o :
	$(CC) $(CFLAGS) -o tests.o tests.c -c

$(PROGNAME) : $(OBJS) tests.o
	gcc -o $(PROGNAME) $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf *.o *.dat $(PROGNAME)

debug:
	make clean && make
