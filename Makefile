DIR_SRC := src
DIR_BIN := bin

CC := gcc

BINS := $(patsubst ${DIR_SRC}/%.c,${DIR_BIN}/%,$(wildcard ${DIR_SRC}/*.c))

FLAGS := -g -Wall --pedantic

all: ${BINS}


${DIR_BIN}/% : ${DIR_SRC}/%.c Makefile | ${DIR_BIN}
	${CC} $(filter %.c %.h,$^) -o $@ ${FLAGS}


${DIR_BIN} :
	mkdir -p $@
