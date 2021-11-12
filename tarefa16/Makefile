CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g
LDFLAGS = -lm

.PHONY: all clean

all: rpg

rpg: rpg.c
	gcc $(CFLAGS) *.c -o rpg $(LDFLAGS)

clean:
	rm -f rpg
