CC = gcc
CFLAGS = -Wall -g -no-pie -Iheaders
LDFLAGS = 

OBJS = basicio.o parser.o shell.o process.o str.o task.o memory.o utils.o

# Default target to build everything (without cleaning)
all: runshell

# Build the runshell executable
runshell: $(OBJS) runshell.o
	$(CC) $(CFLAGS) -o runshell runshell.o $(OBJS)

# Rule to build any test program (where N can be any string, not just a number)
test%: $(OBJS) tests/test%.o
	$(CC) $(CFLAGS) -o $@ $(OBJS) tests/test$*.o 

# Rule to compile any .c file located in directories starting with "test"
tests/%.o: tests/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile individual object files for the project
shell.o: src/shell.c headers/shell.h headers/defs.h
	$(CC) $(CFLAGS) -c src/shell.c -o shell.o

runshell.o: src/runshell.c headers/shell.h headers/defs.h
	$(CC) $(CFLAGS) -c src/runshell.c -o runshell.o

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

process.o: src/process.c headers/process.h headers/defs.h
	$(CC) $(CFLAGS) -c src/process.c -o process.o

utils.o: src/utils.c headers/utils.h headers/defs.h
	$(CC) $(CFLAGS) -c src/utils.c -o utils.o

# Clean target (optional: run this separately)
clean:
	rm -f *.o tests/*.o
