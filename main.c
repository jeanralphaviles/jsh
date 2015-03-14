#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "ast.h"

extern int yyparse();
extern int yy_scan_string(const char *);
//extern int yy_delete_buffer(int buf);
extern struct AstRoot* astRoot;

int main(int argc, char* argv[]) {
	char line[MAX_LENGTH];

	while (1) {
		printf("jsh> ");
		if (!fgets(line, MAX_LENGTH, stdin))
			break;

		int buf = yy_scan_string(line);
		int returnVal = yyparse();
		if (returnVal == 0) {
			if (astRoot != NULL) {
				executeAstRoot(astRoot);
			}
		}
		//yy_delete_buffer(buf);
	}
	return 0;
}
