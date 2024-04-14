CC=gcc
CFLAGS=-Wall -Wfatal-errors -I$(HOME)/Documents/Ensta-Paris/1A/Informatique/IN103/libin103-1.4.2/include
LDFLAGS=-L$(HOME)/Documents/Ensta-Paris/1A/Informatique/IN103/libin103-1.4.2/source -lin103

all: day16-2.x day16-1.x day16-brouillon.x

day16-brouillon.x: day16-brouillon.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

day16-1.x: day16-1.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

day16-2.x: day16-2.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(LDFLAGS)

clean:
	rm -f *.o

realclean: clean
	rm -f *.x