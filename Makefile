all: y.tab.o lex.yy.o main.o queue.o utils.o aliastable.o builtins.o ast.o 
	cc lex.yy.o y.tab.o main.o queue.o utils.o aliastable.o builtins.o ast.o -o main -g

y.tab.h: grammar.y
	yacc -dv grammar.y

y.tab.c: grammar.y
	yacc -dv grammar.y

y.tab.o: y.tab.h y.tab.c
	cc -c y.tab.c -g

lex.yy.c: lexxer.l
	lex lexxer.l

lex.yy.o: lex.yy.c
	cc -c lex.yy.c -g

builtins.o: builtins.h builtins.c
	cc -c builtins.c -g

queue.o: queue.h queue.c
	cc -c queue.c -g

ast.o: ast.h ast.c
	cc -c ast.c -g

aliastable.o: aliastable.h aliastable.c
	cc -c aliastable.c -g

utils.o: utils.c
	cc -c utils.c -g

main.o: main.c
	cc -c main.c -g

clean:
	rm -rf lex.yy.c y.tab.c y.tab.h main *.o *.output *.gch
