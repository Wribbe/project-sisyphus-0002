DIR_SRC := src
DIR_BIN := bin

CC := gcc

BINS := $(patsubst ${DIR_SRC}/%.c,${DIR_BIN}/%,$(wildcard ${DIR_SRC}/*.c))

FLAGS := -I. -g -Wall --pedantic -lX11 -lGL

all: ${BINS}


${DIR_BIN}/% : ${DIR_SRC}/%.c lib/graphics.h lib/graphics.c Makefile | ${DIR_BIN}
	${CC} $(filter %.c,$^) -o $@ ${FLAGS}



${DIR_BIN} :
	mkdir -p $@
