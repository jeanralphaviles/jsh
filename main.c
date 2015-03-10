#include <stdio.h>
#include <stdlib.h>

#define MAX_LENGTH 1024

extern int yyparse();
extern int yylex();

int main(int argc, char* argv[]) {
	//char* cmd;
	//char line[MAX_LENGTH];

	while(1) {
		printf("jsh> ");
		//if (!fgets(line, MAX_LENGTH, stdin)) break;
		//system(line);
		int returnVal = yyparse();
		printf("The return value of yyparse() is: %d\n", returnVal);
		//int returnVal = yylex();
		//printf("The return value of yylex() is: %d\n", returnVal);
	}
	return 0;
}
