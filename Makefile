CC=gcc
SRC=main.c graph.c list.c
HDR=graph.h list.h

all: $(SRC) $(HDR)
	$(CC) $(SRC) -ggdb -o tree

clean:
	rm tree
