CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g
LDFLAGS = -lm

.PHONY: all clean

all: imposto

imposto: imposto.c
	gcc $(CFLAGS) imposto.c -o imposto $(LDFLAGS)

clean:
	rm -f imposto
