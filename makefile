# OBJS = lex.yy.o y.tab.o cactus.o
cactus: .o
	gcc *.o -o cactus
.o: y.tab.c cactus.c lex.yy.c
	gcc -c *.c

lex.yy.c: cactus.l y.tab.h
	flex *.l

y.tab.c: cactus.y
	yacc -dv *.y
	
y.tab.h: cactus.y
	yacc -dv *.y

# y.tab.h: cactus.y
# 	yacc -dv cactus.y
# y.tab.c: cactus.y
# 	yacc -dv cactus.y
# lex.yy.c: cactus.l cactus.y y.tab.h
# 	flex cactus.l
# y.tab.o: y.tab.c 
# 	gcc -c  y.tab.c 
# lex.yy.o: lex.yy.c 
# 	gcc -c  lex.yy.c
# cactus.o: cactus.c
# 	gcc -c cactus.c

clean:
	rm -f *.o y.tab.h y.tab.c lex.yy.c *.output cactus