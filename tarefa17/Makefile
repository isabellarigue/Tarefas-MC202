CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g
LDFLAGS = -lm

.PHONY: all clean

all: def_capitais

def_capitais: def_capitais.c
	gcc $(CFLAGS) *.c -o def_capitais $(LDFLAGS)

clean:
	rm -f def_capitais
