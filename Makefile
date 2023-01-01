CC=gcc
CFLAGS=-g
# DIRS
SRC=src/
INCLUDES=$(SRC)/include/
EXE=cats

# OBJECTS=$(SRC)/objs/

sources=$(wildcard src/*.c)
objects=$(sources:.c=.o)

$(EXE): $(objects)
	gcc $(objects) $(flags) -o $(EXE)

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@