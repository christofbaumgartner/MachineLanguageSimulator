CC=gcc 
CFLAGS = -ansi -pedantic -Wall -Wextra

program: main.o simulator.o
	$(CC) -o program main.o simulator.o $(CFLAGS)

main.o: main.c
	$(CC) -c main.c $(CFLAGS)

interpreter.o: simulator.c
	$(CC) -c simulator.c $(CFLAGS)