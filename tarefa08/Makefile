CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g
LDFLAGS = -lm

.PHONY: all clean

all: boate

boate: boate.c
	gcc $(CFLAGS) boate.c -o boate $(LDFLAGS)

clean:
	rm -f boate
