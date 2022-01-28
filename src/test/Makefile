YACC = bison
LEX  = flex
CC   = gcc
CFLAGS = -std=c11 -g -Wall -Wextra -rdynamic $(UDEF)

all: test

run: test
	./test_string

test: test_string

test_string: string_test.c test.h ../string.h ../string.c
	$(CC) $(CFLAGS) ../string.c string_test.c -o test_string

clean:
	rm -f test_string
	rm -rf *.dSYM
