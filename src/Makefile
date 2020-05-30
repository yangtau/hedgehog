YACC = yacc
LEX  = flex
CC   = gcc
CFLAGS = -std=gnu99 -g -Wall -Wextra

SRC := $(filter-out yy.hedgehog.c lex.hedgehog.c format.c main.c, $(wildcard *.c))
OBJS := $(SRC:.c=.o)

all: hg formater

hg: main.o parser.o $(OBJS)
	$(CC) $(CFLAGS) -o hg main.o parser.o $(OBJS)

formater: format.o
	$(CC) $(CFLAGS) -o formater format.o parser.o $(OBJS)

format.o: format.c
	$(CC) $(CFLAGS) -c format.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

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
