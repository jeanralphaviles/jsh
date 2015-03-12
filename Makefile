all: y.tab.o lex.yy.o main.o queue.o
	cc lex.yy.o y.tab.o main.o -o main

y.tab.h: grammar.y
	yacc -dv grammar.y

y.tab.c: grammar.y
	yacc -dv grammar.y

y.tab.o: y.tab.h y.tab.c
	cc -c y.tab.c

lex.yy.c: lexxer.l
	lex lexxer.l

lex.yy.o: lex.yy.c
	cc -c lex.yy.c

queue.o: queue.h queue.c
	cc -c queue.c

main.o: main.c
	cc -c main.c

clean:
	rm -rf lex.yy.c y.tab.c y.tab.h main *.o *.output
