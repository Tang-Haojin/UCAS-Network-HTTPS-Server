CC = gcc
CFLAGS = -pthread -lssl -lcrypto

INCLUDE = include
SRC = $(wildcard *.c) $(wildcard ${INCLUDE}/*.h)

main: ${SRC}
	${CC} $^ ${CFLAGS} -I${INCLUDE} -o main

clean:
	rm -rf main

.PHONY: clean
