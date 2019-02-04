cactus: cactus.c lex.yy.c y.tab.c
	gcc *.c -o cactus
y.tab.c: cactus.y
	yacc -dv *.y
lex.yy.c: y.tab.c
	lex *.l
clean:
	rm -f *.o y.tab.h y.tab.c lex.yy.c *.output cactus