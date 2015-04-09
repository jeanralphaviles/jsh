#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ast.h"
#include "aliastable.h"
#include "defines.h"
#include "readline/readline.h"
#include "readline/history.h"
#include "utils.h"

extern int yyparse();
extern int yy_scan_string(const char *);
extern struct AstRoot* astRoot;

void init(void);
char* getPrompt();
void printWelcome(void);

int main(int argc, char* argv[]) {
  init();
  printWelcome();
  char* line;
  rl_bind_key('\t', rl_abort); // Disable tab to complete
  rl_bind_key('\e', rl_complete); // Set Esacpe to complete

  while ((line = readline(getPrompt())) != NULL) {
    if (strlen(line) == 0 || strcmp(line, "\n") == 0) {
      continue;
    }
    char* cmdline = (char*)malloc(MAX_LENGTH);
    strcpy(cmdline, line);
    aliasSub(cmdline);
    astRoot = NULL;

    int buf = yy_scan_string(cmdline);
    int returnVal = yyparse();
    if (returnVal == 0) {
      if (astRoot != NULL) {
        add_history(line);
        executeAstRoot(astRoot);
        free(astRoot); // Huge memory leak here :)
      }
    } else {
      const char* oldTermColor = setTermColor(stderr, KRED);
      fprintf(stderr, "An error occurred\n");
      setTermColor(stderr, oldTermColor);
    }
    setTermColor(stderr, KCYN);
    free(cmdline);
    free(line);
  }
  return 0;
}

void init(void) {
  signal(SIGINT, SIG_IGN);  /* disable ctrl-C */
  signal(SIGQUIT, SIG_IGN); /* disable ctrl-\ */
  signal(SIGTSTP, SIG_IGN); /* disable ctrl-Z */
}

void printWelcome(void) {
	fprintf(stderr, "===========Welcome to JSH!===========\n");
	fprintf(stderr, "Written by JR Aviles and Joe Liccini.\n");
}

char* getPrompt() {
  static char* prompt;
  if (prompt != NULL) {
    free(prompt);
  }
  prompt = (char*)malloc(100);
  strcpy(prompt, KCYN);
  strcat(prompt, "jsh ");
  strcat(prompt, KGRN);
  strcat(prompt, (char*)(unsigned long)get_current_dir_name());
  strcat(prompt, KCYN);
  strcat(prompt, " $ ");
  strcat(prompt, KNRM);
  return prompt;
}
