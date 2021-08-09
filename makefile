TARGET=bin/tetris-chaos
OBJS=lib/gameManager.o lib/rotations.o lib/interface.o lib/menus.o lib/main.o


GC=gcc
LD=gcc

WARN=-Wall
COMPILE=-c
DEBUG=-g
NCURS=-lncurses
 
GCFLAGS=$(WARN) $(DEBUG) $(COMPILE)
LDFLAGS=$(WARN) $(DEBUG) $(NCURS)


all: bin lib $(OBJS)
	$(LD) $(LDFLAGS) -o $(TARGET) $(OBJS)

lib/main.o: src/main.c
	$(GC) $(GCFLAGS) -o lib/main.o src/main.c

lib/gameManager.o: src/gameManager.c
	$(GC) $(GCFLAGS) -o lib/gameManager.o src/gameManager.c

lib/rotations.o: src/rotations.c
	$(GC) $(GCFLAGS) -o lib/rotations.o src/rotations.c

lib/interface.o: src/interface.c
	$(GC) $(GCFLAGS) -o lib/interface.o src/interface.c

lib/menus.o: src/menus.c
	$(GC) $(GCFLAGS) -o lib/menus.o src/menus.c

bin:
	mkdir bin
lib:
	mkdir lib

clean:
	rm lib/*.o $(TARGET)