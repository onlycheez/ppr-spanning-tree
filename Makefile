CC=gcc
SRC=main.c graph.c
HDR=graph.h

all: $(SRC) $(HDR)
	$(CC) $(SRC) -o tree

clean:
	rm tree
