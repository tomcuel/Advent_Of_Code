include ../Makefile.common

CC=gcc

ifeq ($(debug), true)
CFLAGS=-Wall -Werror -Wdeprecated -std=c99 -pedantic -O0 -g -I../include -DDEBUG
else
CFLAGS=-Wall -Werror -Wdeprecated -std=c99 -pedantic -O2 -I../include
endif

all: static
# 	generic_uf.o
static: $(LIST_OBJ) generic_list.o 					\
	$(STACK_OBJ) generic_stack.o 					\
	$(QUEUE_OBJ) generic_queue.o 					\
	$(SET_OBJ) generic_set.o 					\
	$(HEAP_OBJ) generic_heap.o 					\
	integer_uf.o							\
	$(BITREE_OBJ) generic_bitree.o 					\
	$(BITREEALG_OBJ) generic_bitreealg.o		 		\
	$(BISTREE_OBJ) generic_bistree.o 				\
	$(GRAPH_OBJ) generic_graph.o 					\
	$(GRAPHALG_OBJ) 						\
	generic_bfs.o generic_dfs.o generic_mst.o generic_shortest.o
	ar rcs libin103.a $^
	chmod 755 libin103.a

# dynamic: $(LIST_OBJ) generic_list.o $(STACK_OBJ) $(QUEUE_OBJ) $(SET_OBJ) $(HEAP_OBJ) $(BITREE_OBJ) generic_bitree.o
# 	$(CC) -dynamiclib -o libin103.so $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	@rm -f $(LIST_SRC) $(STACK_SRC) $(QUEUE_SRC) $(SET_SRC) $(HEAP_SRC) $(BITREE_SRC) $(BITREEALG_SRC) $(BISTREE_SRC) $(GRAPH_SRC) $(GRAPHALG_SRC)
	@rm -f libin103.so libin103.a *.o
	@rm -f *~

.PHONY: all clean
