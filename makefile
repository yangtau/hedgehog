hedgehog: hedgehog.c lex.yy.c y.tab.c
	gcc *.c -o hedgehog
y.tab.c: hedgehog.y
	yacc -dv *.y
lex.yy.c: y.tab.c
	lex *.l
clean:
	rm -f *.o y.tab.h y.tab.c lex.yy.c *.output hedgehog