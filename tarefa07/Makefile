CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g
LDFLAGS = -lm

.PHONY: all clean run

all: arranhaceu

arranhaceu: arranhaceu.c
	gcc $(CFLAGS) arranhaceu.c -o arranhaceu $(LDFLAGS)

run: all
	./arranhaceu < testes/exemplo.in

clean:
	rm -f arranhaceu