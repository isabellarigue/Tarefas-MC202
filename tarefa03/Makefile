CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g
LDFLAGS = -lm

.PHONY: all clean

all: classificar

classificar: classificar.c
	gcc $(CFLAGS) classificar.c -o classificar $(LDFLAGS)

clean:
	rm -f classificar
