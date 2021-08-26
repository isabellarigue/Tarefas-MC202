CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g

.PHONY: all clean

all: ribossomo

minecraft: ribossomo.c
	gcc $(CFLAGS) ribossomo.c -o ribossomo $(LDFLAGS)

clean:
	rm -f ribossomo
