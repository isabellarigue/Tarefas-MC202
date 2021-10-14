CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g
LDFLAGS = -lm

.PHONY: all clean

all: busca

busca: busca.c
	gcc $(CFLAGS) *.c -o busca $(LDFLAGS)

clean:
	rm -f busca
