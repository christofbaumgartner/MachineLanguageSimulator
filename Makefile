CC=gcc 
CFLAGS = -ansi -pedantic -Wall -Wextra

program: main.o interpreter.o
	$(CC) -o program main.o interpreter.o $(CFLAGS)

main.o: main.c
	$(CC) -c main.c $(CFLAGS)

interpreter.o: interpreter.c
	$(CC) -c interpreter.c $(CFLAGS)