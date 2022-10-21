CC = gcc
CFLAGS = -pthread -lssl -lcrypto

SRC = $(wildcard *.c)
INCLUDE = include

main: ${SRC}
	${CC} $^ ${CFLAGS} -I${INCLUDE} -o main

clean:
	rm -rf main

.PHONY:
	clean
