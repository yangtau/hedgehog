YACC = bison
LEX  = flex
CC   = gcc
CFLAGS = -std=c11 -g -Wall -Wextra -rdynamic $(UDEF)

SRC := $(filter-out yy.hedgehog.c lex.hedgehog.c format.c, $(wildcard *.c))
OBJS := $(SRC:.c=.o)

.PHONY : test run_test clean

all: formater test

test:
	make -C test

run_test:
	make -C test run

formater: format.c $(OBJS) parser.o
	$(CC) $(CFLAGS) -o $@ $^

parser.o: parse.y lex.l
	$(YACC) -v -d -o yy.hedgehog.c parse.y
	$(LEX) -o lex.hedgehog.c lex.l
	$(CC) $(CFLAGS) -c yy.hedgehog.c -o $@

$(OBJS):%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f yy.hedgehog.* lex.hedgehog.* *.o
	rm -f formater
	rm -f hg
	rm -rf *.dSYM
	make -C test $@
