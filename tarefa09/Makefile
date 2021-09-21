CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g
LDFLAGS = -lm

.PHONY: all clean

all: equipes

equipes: equipes.c
	gcc $(CFLAGS) *.c -o equipes $(LDFLAGS)

clean:
	rm -f equipes
