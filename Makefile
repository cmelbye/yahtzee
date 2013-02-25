CC=gcc
CFLAGS=-ansi -Wall -pedantic -MMD
LDFLAGS=-lm

SRCS=$(wildcard *.c)
OBJS=$(SRCS:%.c=%.o)

all: yahtzee

yahtzee: $(OBJS)

clean:
	$(RM) *.o *.d yahtzee

.PHONY: all clean

-include *.d

