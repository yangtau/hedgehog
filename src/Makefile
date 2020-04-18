YACC = yacc
LEX  = flex
CC   = gcc
CFLAGS = -std=gnu99 -g -Wall


yy.hedgehog.c: parse.y
	$(YACC) -d -o yy.hedgehog.c parse.y

lex.hedgehog.c: lex.l yy.hedgehog.h
	$(LEX) -o lex.hedgehog.c lex.l

parser.o: yy.hedgehog.c lex.hedgehog.o
	$(CC) $(CFLAGS) -c yy.hedgehog.c -o parser.o


clean:
	rm -f yy.hedgehog.* lex.hedgehog.* *.o
