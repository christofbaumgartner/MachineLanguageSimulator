CC=gcc 
CFLAGS = -ansi -pedantic -Wall -Wextra

program: main.o simulator.o cmd_writer.o global.o
	$(CC) -o program main.o simulator.o cmd_writer.o global.o $(CFLAGS)

main.o: main.c
	$(CC) -c main.c $(CFLAGS)

simulator.o: simulator.c
	$(CC) -c simulator.c $(CFLAGS)

cmd_writer.o: cmd_writer.c
	$(CC) -c cmd_writer.c $(CFLAGS)
	
global.o: global.c
	$(CC) -c global.c $(CFLAGS)