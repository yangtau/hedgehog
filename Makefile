CC = gcc
CFLAGS = -std=gnu99 -g -Wall 

MKDIR = mkdir -p

bin = bin
src = $(wildcard src/*.c)
objs := $(src:.c=.o)
target = hegdgehog

.PHONY: all
all: $(objs)

$(target):$(objs)
	$(CC) $(CFLAGS) -o $(bin)/$(target) $(bin)/*.o

$(objs):%.o: %.c
	$(MKDIR) $(bin)
	$(CC) $(CFLAGS) -c $< -o $(bin)/$(notdir $@)

.PHONY: clean
clean:
	rm -rf $(bin)/*
