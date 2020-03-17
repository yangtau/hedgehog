CC = gcc
CFLAGS = -std=gnu99 -g -Wall 
YACC = yacc -d
LEX = flex

MKDIR = mkdir -p

bin = bin
src = $(wildcard src/*.c)
objs := $(src:.c=.o)
target = hedgehog

.PHONY: all
all: $(target)

$(target):$(objs) parse
	$(CC) $(CFLAGS) -o $(bin)/$(target) $(bin)/*.o

$(objs):%.o: %.c
	$(MKDIR) $(bin)
	$(CC) $(CFLAGS) -c $< -o $(bin)/$(notdir $@)

parse: src/hedgehog.y src/hedgehog.l
	$(LEX) -o src/lex.yy.c src/hedgehog.l
	$(YACC) src/hedgehog.y -o src/y.tab.c
	$(CC) -c src/y.tab.c -o $(bin)/y.tab.o
	$(CC) -c src/lex.yy.c -o $(bin)/lex.yy.o

.PHONY: clean
clean:
	rm -rf $(bin)/*
	rm -f src/lex.yy.c src/y.tab.c
