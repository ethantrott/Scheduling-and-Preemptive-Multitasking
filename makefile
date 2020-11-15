CFLAGS = -std=c99 -g
  
all: Project1

Project1: main.o Opcodes.o
	$(CC) $(CFLAGS) -o Project1 Opcodes.o main.o

main.o: main.c Vars.h
	$(CC) $(CFLAGS) -c main.c -o main.o

Opcodes.o: Opcodes.c Vars.h
	$(CC) $(CFLAGS) -c Opcodes.c -o Opcodes.o

