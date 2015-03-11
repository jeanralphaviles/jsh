#include <stdio.h>
#include <stdlib.h>

#define MAX_LENGTH 1024

extern int yyparse();
extern int yylex();
extern int yy_scan_string(const char *);
extern int yy_delete_buffer(int buf);

int main(int argc, char* argv[]) {
	//char* cmd;
	char line[MAX_LENGTH];

	while(1) {
		printf("jsh> ");
		if (!fgets(line, MAX_LENGTH, stdin)) break;
		//system(line);
		
		int buf = yy_scan_string(line);
		int returnVal = yyparse();
		yy_delete_buffer(buf);
		//printf("The return value of yyparse() is: %d\n", returnVal);

	}
	return 0;
}
