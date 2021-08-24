CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g
LDFLAGS = -lm

.PHONY: all clean

all: planejar

planejar: main.o planejar.o
	gcc $(CFLAGS) main.o planejar.o -o planejar $(LDFLAGS)

main.o: main.c planejar.h
	gcc $(CFLAGS) -c main.c

planejar.o: planejar.c planejar.h
	gcc $(CFLAGS) -c planejar.c

clean:
	rm -f planejar planejar.o main.o
