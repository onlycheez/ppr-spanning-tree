CC=gcc
SRC=main.c graph.c
HDR=graph.h stack.h

all: $(SRC) $(HDR)
	$(CC) $(SRC) -ggdb -o tree

clean:
	rm tree
