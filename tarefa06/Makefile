CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g

.PHONY: all clean

all: minecraft

minecraft: minecraft.c minecraft.h
	gcc $(CFLAGS) minecraft.c -o minecraft $(LDFLAGS)

clean:
	rm -f minecraft
