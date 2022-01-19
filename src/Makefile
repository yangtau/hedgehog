YACC = bison
LEX  = flex
CC   = gcc
CFLAGS = -std=c11 -g -Wall -Wextra -rdynamic $(UDEF)

SRC := $(filter-out yy.hedgehog.c lex.hedgehog.c format.c, $(wildcard *.c))
OBJS := $(SRC:.c=.o)

all: formater

formater: format.c $(OBJS) parser.o
	$(CC) $(CFLAGS) -o formater format.c parser.o $(OBJS)

parser.o: parse.y lex.l
	$(YACC) -v -d -o yy.hedgehog.c parse.y
	$(LEX) -o lex.hedgehog.c lex.l
	$(CC) $(CFLAGS) -c yy.hedgehog.c -o parser.o

$(OBJS):%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f yy.hedgehog.* lex.hedgehog.* *.o
	rm -f formater
	rm -f hg
	rm -rf *.dSYM
