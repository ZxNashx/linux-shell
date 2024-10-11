
CC = gcc
CFLAGS = -Wall -g -no-pie -Iheaders
LDFLAGS = 

OBJS = basicio.o parser.o shell.o runshell.o process.o str.o task.o memory.o

all: runshell clean

runshell: $(OBJS)
	$(CC) $(CFLAGS) -o runshell $(OBJS)

basicio.o: src/basicio.c headers/basicio.h headers/defs.h
	$(CC) $(CFLAGS) -c src/basicio.c -o basicio.o

str.o: src/str.c headers/str.h headers/defs.h
	$(CC) $(CFLAGS) -c src/str.c -o str.o

task.o: src/task.c headers/task.h headers/defs.h
	$(CC) $(CFLAGS) -c src/task.c -o task.o

memory.o: src/memory.c headers/memory.h headers/defs.h
	$(CC) $(CFLAGS) -c src/memory.c -o memory.o

parser.o: src/parser.c headers/parser.h headers/defs.h
	$(CC) $(CFLAGS) -c src/parser.c -o parser.o

shell.o: src/shell.c headers/shell.h headers/defs.h
	$(CC) $(CFLAGS) -c src/shell.c -o shell.o

process.o: src/process.c headers/process.h headers/defs.h
	$(CC) $(CFLAGS) -c src/process.c -o process.o

runshell.o: src/runshell.c headers/shell.h headers/defs.h
	$(CC) $(CFLAGS) -c src/runshell.c -o runshell.o

clean:
	rm -f $(OBJS)
