CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g
LDFLAGS = -lm

.PHONY: all clean

all: piramide

piramide: piramide.c
	gcc $(CFLAGS) *.c -o piramide $(LDFLAGS)

clean:
	rm -f piramide
