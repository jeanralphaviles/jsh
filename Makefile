all: lex.yy.c y.tab.c y.tab.h
	cc lex.yy.c y.tab.c -o main

y.tab.h: y.tab.c

y.tab.c: grammar.y
	yacc -dv grammar.y

lex.yy.c: lexxer.l
	lex lexxer.l

clean:
	rm -rf lex.yy.c y.tab.c y.tab.h main
