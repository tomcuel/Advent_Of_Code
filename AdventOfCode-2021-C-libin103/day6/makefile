CC=gcc
CFLAGS=-Wall -Wfatal-errors -I$(HOME)/Documents/Ensta-Paris/1A/Informatique/IN103/libin103-1.4.2/include
LDFLAGS=-L$(HOME)/Documents/Ensta-Paris/1A/Informatique/IN103/libin103-1.4.2/source -lin103

all: day6-2.x day6-1.x

day6-1.x: day6-1.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

day6-2.x: day6-2.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(LDFLAGS)

clean:
	rm -f *.o

realclean: clean
	rm -f *.x