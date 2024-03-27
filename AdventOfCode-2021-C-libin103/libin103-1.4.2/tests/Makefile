include ../Makefile.common

BINS= test-lists.x test-lists-generic.x test-set-generic.x test-bitree.x test-uf.x test-bitree-generic.x test-bistree.x test-graph-generic.x test-graph.x

CC=gcc

ifeq ($(debug), true)
CFLAGS=-Wall -Werror -Wdeprecated -std=c99 -pedantic -O0 -g -Ilibtap-master -I../include -DDEBUG
else
CFLAGS=-Wall -Werror -Wdeprecated -std=c99 -pedantic -O2 -Ilibtap-master -I../include
endif

ifeq ($(debug), true)
LDFLAGS=-Llibtap-master -ltap -L../source -lin103
else
LDFLAGS=-Llibtap-master -ltap -L../source -lin103
endif

all: lib tests

tests: $(BINS)

run: $(BINS)
	for pgm in $(BINS); do ./$$pgm; done

lib:
	make -C libtap-master libtap.a

%.x: %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	@rm -f *~
	@rm -f *.o
	@rm -rf *.dSYM

realclean: clean
	@rm -f *.x
	make -C libtap-master clean
