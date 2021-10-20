CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g
LDFLAGS = -lm

.PHONY: all clean

all: dns

dns: dns.c
	gcc $(CFLAGS) *.c -o dns $(LDFLAGS)

test: dns
	./dns < testes/example.in

clean:
	rm -f dns
