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

$(target):$(objs) $(bin)/y.tab.o $(bin)/lex.yy.o
	$(CC) $(CFLAGS) -o $(bin)/$(target) $(bin)/*.o

$(objs):%.o: %.c
	$(MKDIR) $(bin)
	$(CC) $(CFLAGS) -c $< -o $(bin)/$(notdir $@)

$(bin)/lex.yy.o: src/hedgehog.l
	$(LEX) -o src/lex.yy.c src/hedgehog.l
	$(CC) $(CFLAGS) -c src/lex.yy.c -o $(bin)/lex.yy.o


$(bin)/y.tab.o: src/hedgehog.y
	$(YACC) src/hedgehog.y -o src/y.tab.c
	$(CC) $(CFLAGS) -c src/y.tab.c -o $(bin)/y.tab.o

.PHONY: clean
clean:
	rm -rf $(bin)/*
	rm -f src/lex.yy.c src/y.tab.c
