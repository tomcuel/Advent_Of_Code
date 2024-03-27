include Makefile.common

CC=gcc
CFLAGS=-Wall -Werror -Wdeprecated -std=c99 -pedantic -O2

all: lib

code: $(LISTS) $(STACKS) $(QUEUES) $(SETS) $(HEAPS) $(BITREES) $(BITREEALGS) $(BISTREES) $(GRAPHS) $(GRAPHALG_HEADER) $(GRAPHALG_SRC)

# remove last #endif directive of a file ?
# | indent -br -pcs -nsc

integer_%.h: templates/%_h.template
	sed -e 's/<TYPE>/int/g' -e 's/<NAME>/integer/g' $< > include/$@

integer_%.c: templates/%_c.template
	sed -e 's/<TYPE>/int/g' -e 's/<NAME>/integer/g' $< > source/$@

real_%.h: templates/%_h.template
	sed -e 's/<TYPE>/double/g' -e 's/<NAME>/real/g' $< > include/$@

real_%.c: templates/%_c.template
	sed -e 's/<TYPE>/double/g' -e 's/<NAME>/real/g' $< > source/$@

character_%.h: templates/%_h.template
	sed -e 's/<TYPE>/char/g' -e 's/<NAME>/character/g' $< > include/$@

character_%.c: templates/%_c.template
	sed -e 's/<TYPE>/char/g' -e 's/<NAME>/character/g' $< > source/$@

lib: code
	make -C source/ all

check: code lib
	make -C tests
	make -C tests run

doc: code lib
	cd include; doxygen

clean:
	make -C include clean
	make -C source clean
	make -C tests clean
	[ -d ./examples ] && make -C examples clean || echo "File missing"
	@rm -f *~
	@rm -f a.out

realclean: clean
	rm -rf include/html
	make -C tests realclean
	[ -d ./examples ] &&  make -C examples realclean || echo "File missing"
	rm -rf include/html

.PHONY: all clean check lib doc
