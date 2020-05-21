YACC = yacc
LEX  = flex
CC   = gcc
CFLAGS = -std=gnu99 -g -Wall

SRC := $(filter-out yy.hedgehog.c lex.hedgehog.c, $(wildcard *.c))
OBJS := $(SRC:.c=.o)

all: hg

hg: parser.o $(OBJS)
	$(CC) -o hg parser.o $(OBJS)

parser.o: parse.y lex.l
	$(YACC) -v -d -o yy.hedgehog.c parse.y
	$(LEX) -o lex.hedgehog.c lex.l
	$(CC) $(CFLAGS) -c yy.hedgehog.c -o parser.o

$(OBJS):%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f yy.hedgehog.* lex.hedgehog.* *.o hg tag
