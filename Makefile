CC=/usr/lib64/openmpi/bin/mpicc
SRC=main.c graph.c list.c
HDR=graph.h list.h
LIB=-lm

all: $(SRC) $(HDR)
	$(CC) $(SRC) $(LIB) -ggdb -o tree

clean:
	rm tree
