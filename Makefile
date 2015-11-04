CC=gcc
SRC=main.c list.c
HDR=list.h

all: $(SRC) $(HDR)
	$(CC) $(SRC) -o tree

clean:
	rm tree
