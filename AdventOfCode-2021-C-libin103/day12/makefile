CC=gcc
CFLAGS=-Wall -Wfatal-errors -I$(HOME)/Documents/Ensta-Paris/1A/Informatique/IN103/libin103-1.4.2/include
LDFLAGS=-L$(HOME)/Documents/Ensta-Paris/1A/Informatique/IN103/libin103-1.4.2/source -lin103

all: day12-2.x day12-1.x day12-1_using_lib.x day12-2_using_lib.x 

day12-2_using_lib.x : day12-2_using_lib.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

day12-1_using_lib.x: day12-1_using_lib.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

day12-1.x: day12-1.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

day12-2.x: day12-2.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(LDFLAGS)

clean:
	rm -f *.o

realclean: clean
	rm -f *.x