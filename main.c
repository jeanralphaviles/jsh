#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "aliastable.h"
#include "defines.h"
#include "utils.h"

extern int yyparse();
extern int yy_scan_string(const char *);
extern struct AstRoot* astRoot;

void init(void);

int main(int argc, char* argv[]) {
  char line[MAX_LENGTH];
  init();

  while (1) {
    setTermColor(stderr, KCYN);
    fprintf(stderr, "jsh> ");
    setTermColor(stderr, KNRM);
    if (!fgets(line, MAX_LENGTH, stdin)) {
      break;
    }
    aliasSub(line);

    int buf = yy_scan_string(line);
    int returnVal = yyparse();
    if (returnVal == 0) {
      if (astRoot != NULL) {
        executeAstRoot(astRoot);
        free(astRoot); // Huge memory leak here :)
      }
    } else {
      const char* oldTermColor = setTermColor(stderr, KRED);
      fprintf(stderr, "An error occurred\n");
      setTermColor(stderr, oldTermColor);
    }
  }
  return 0;
}

void init(void) {
  signal(SIGINT, SIG_IGN);  /* disable ctrl-C */
  signal(SIGQUIT, SIG_IGN); /* disable ctrl-\ */
  signal(SIGTSTP, SIG_IGN); /* disable ctrl-Z */
}
