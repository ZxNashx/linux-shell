CC = gcc
CFLAGS = -Wall -g -no-pie -Iheaders
LDFLAGS = 

OBJS = basicio.o parser.o process.o str.o utils.o task.o signal_handler.o

# Default target to build everything
all: runshell clean

# Build the runshell executable
runshell: $(OBJS) runshell.o
	$(CC) $(CFLAGS) -o runshell.out runshell.o $(OBJS)

test%: $(OBJS) tests/test%.o
	$(CC) $(CFLAGS) -o $@.out $(OBJS) tests/test$*.o 

tests/%.o: tests/%.c
	$(CC) $(CFLAGS) -c $< -o $@

runshell.o: src/runshell.c  headers/defs.h
	$(CC) $(CFLAGS) -c src/runshell.c -o runshell.o

basicio.o: src/basicio.c headers/basicio.h headers/defs.h
	$(CC) $(CFLAGS) -c src/basicio.c -o basicio.o
signal_handler.o: src/signal_handler.c headers/signal_handler.h
	$(CC) $(CFLAGS) -c src/signal_handler.c -o signal_handler.o

str.o: src/str.c headers/str.h headers/defs.h
	$(CC) $(CFLAGS) -c src/str.c -o str.o

task.o: src/task.c headers/task.h headers/defs.h
	$(CC) $(CFLAGS) -c src/task.c -o task.o

parser.o: src/parser.c headers/parser.h headers/defs.h
	$(CC) $(CFLAGS) -c src/parser.c -o parser.o

process.o: src/process.c headers/process.h headers/defs.h
	$(CC) $(CFLAGS) -c src/process.c -o process.o

utils.o: src/utils.c headers/utils.h headers/defs.h
	$(CC) $(CFLAGS) -c src/utils.c -o utils.o

clean:
	rm -f *.o tests/*.o 
full_clean: clean
	rm -f *.out