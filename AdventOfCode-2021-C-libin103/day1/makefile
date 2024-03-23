FILES=day1-1.c day1-2.c

EXE=$(patsubst %.c, %.x, $(FILES))

CC=gcc 
CFLAGS=-Wall -Wfatal-errors -I$(HOME)/Documents/Ensta-Paris/1A/Informatique/IN103/libin103-1.4.2/include
LDFLAGS=-L$(HOME)/Documents/Ensta-Paris/1A/Informatique/IN103/libin103-1.4.2/source -lin103

all: $(EXE)

%.x: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	@rm -f *~
	@rm -f *.o

realclean: clean
	@rm -f *.x