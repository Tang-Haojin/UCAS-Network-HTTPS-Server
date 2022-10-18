CC = gcc
CFLAGS = -pthread

SRC = $(wildcard *.c)
INCLUDE = include

main: ${SRC}
	${CC} $^ ${CFLAGS} -I${INCLUDE} -o main

clean:
	rm -rf main

.PHONY:
	clean
