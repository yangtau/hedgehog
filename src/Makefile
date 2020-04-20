YACC = yacc
LEX  = flex
CC   = gcc
CFLAGS = -std=gnu99 -g -Wall

SRC := $(wildcard *.c)
OBJS := $(SRC:.c=.o)

all: hg

hg: parser.o $(OBJS)
	$(CC) -o hg parser.o $(OBJS)

yy.hedgehog.c: parse.y
	$(YACC) -v -d -o yy.hedgehog.c parse.y

lex.hedgehog.c: lex.l yy.hedgehog.h
	$(LEX) -o lex.hedgehog.c lex.l

parser.o: yy.hedgehog.c lex.hedgehog.c
	$(CC) $(CFLAGS) -c yy.hedgehog.c -o parser.o

$(OBJS):%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f yy.hedgehog.* lex.hedgehog.* *.o hg
