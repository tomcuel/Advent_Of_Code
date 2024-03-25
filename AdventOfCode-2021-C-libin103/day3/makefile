CC=gcc
CFLAGS=-Wall -Wfatal-errors -I$(HOME)/Documents/Ensta-Paris/1A/Informatique/IN103/libin103-1.4.2/include
LDFLAGS=-L$(HOME)/Documents/Ensta-Paris/1A/Informatique/IN103/libin103-1.4.2/source -lin103

all: day3-2.x day3-1.x

day3-1.x:
	$(CC) $(CFLAGS) -o day3-1.x day3-1.c

day3-2.x: board.o numbers.o results.o day3-2.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(LDFLAGS)

clean:
	rm -f *.o

realclean: clean
	rm -f *.x
