CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g

.PHONY: all clean

all: roteador

roteador: roteador.c
	gcc $(CFLAGS) *.c -o roteador $(LDFLAGS)

clean:
	rm -f roteador
