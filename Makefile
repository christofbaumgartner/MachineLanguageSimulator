CC=gcc 
CFLAGS = -ansi -pedantic -Wall -Wextra

program: main.o simulator.o cmd_writer.o
	$(CC) -o program main.o simulator.o cmd_writer.o $(CFLAGS)

main.o: main.c
	$(CC) -c main.c $(CFLAGS)

simulator.o: simulator.c
	$(CC) -c simulator.c $(CFLAGS)

cmd_writer.o: cmd_writer.c
	$(CC) -c cmd_writer.c $(CFLAGS)