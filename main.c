#include <stdio.h>
#include <stdlib.h>

#define MAX_LENGTH 1024

extern int yyparse();

int main(int argc, char* argv[]) {
	char* cmd;
	char line[MAX_LENGTH];

	while(1) {
		printf("jsh> ");
		if (!fgets(line, MAX_LENGTH, stdin)) break;
		//system(line);
		yyparse();
	}
	return 0;
}
