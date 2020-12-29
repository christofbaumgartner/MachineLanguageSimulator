CC=gcc 
CFLAGS = -ansi -pedantic -Wall -Wextra

program: main_read.o simulator.o
	$(CC) -o program main_read.o simulator.o $(CFLAGS)

main_read.o: main_read.c
	$(CC) -c main_read.c $(CFLAGS)

interpreter.o: simulator.c
	$(CC) -c simulator.c $(CFLAGS)