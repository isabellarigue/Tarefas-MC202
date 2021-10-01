CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g
LDFLAGS = -lm

.PHONY: all clean

all: docas

docas: docas.c
	gcc $(CFLAGS) *.c -o docas $(LDFLAGS)

clean:
	rm -f docas
