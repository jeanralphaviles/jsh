#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "defines.h"

extern int yyparse();
extern int yy_scan_string(const char *);
extern struct AstRoot* astRoot;

void init(void);

int main(int argc, char* argv[]) {
  char line[MAX_LENGTH];
  init();

  while (1) {
    fprintf(stderr, "jsh> ");
    if (!fgets(line, MAX_LENGTH, stdin)) {
      break;
    }

    int buf = yy_scan_string(line);
    int returnVal = yyparse();
    if (returnVal == 0) {
      if (astRoot != NULL) {
        executeAstRoot(astRoot);
        free(astRoot); // Huge memory leak here :)
      }
    } else {
      fprintf(stderr, "An error occurred\n");
    }
  }
  return 0;
}

void init(void) {
  signal(SIGINT, SIG_IGN);  /* disable ctrl-C */
  signal(SIGQUIT, SIG_IGN); /* disable ctrl-\ */
  signal(SIGTSTP, SIG_IGN); /* disable ctrl-Z */
}
