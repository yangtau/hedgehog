YACC = yacc
LEX  = flex
CC   = gcc
CFLAGS = -std=gnu99 -g -Wall -Wextra

SRC := $(filter-out yy.hedgehog.c lex.hedgehog.c format.c main.c hash_test.c, $(wildcard *.c))
OBJS := $(SRC:.c=.o)

all: hg formater

hg: main.c parser.o $(OBJS)
	$(CC) $(CFLAGS) -o hg main.c parser.o $(OBJS)

hash_test: hash_test.c $(OBJS)
	$(CC) $(CFLAGS) -o hash_test hash_test.c $(OBJS)

formater: format.c $(OBJS)
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
	rm -f tags
	rm -f hg
	rm -f hash_test
