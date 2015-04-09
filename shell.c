#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <readline/history.h>
#include <readline/readline.h>

#include "ast.h"
#include "aliastable.h"
#include "defines.h"
#include "utils.h"

extern int yyparse();
extern int yy_scan_string(const char *);
extern struct AstRoot* astRoot;

static void init(void);
static char* getPrompt();
static void printWelcome(void);
static void printRainbow(char*);

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

static void init(void) {
  signal(SIGINT, SIG_IGN);  /* disable ctrl-C */
  signal(SIGQUIT, SIG_IGN); /* disable ctrl-\ */
  signal(SIGTSTP, SIG_IGN); /* disable ctrl-Z */
}

static void printWelcome(void) {
  printRainbow("===========");
  fprintf(stderr, "%sWelcome to %sJSH%s", KRED, KRED, KNRM);
  printRainbow("===========\n");
  fprintf(stderr, "Written by JR Aviles and Joe Liccini.\n\n");
}

static char* getPrompt() {
  static char ignoreString[2] = {RL_PROMPT_START_IGNORE, '\0'};
  static char allowString[2] = {RL_PROMPT_END_IGNORE, '\0'};
  static char* prompt;
  char* cwd = (char*)malloc(100);
  if (prompt != NULL) {
    free(prompt);
  }
  prompt = (char*)malloc(100);
  strcpy(prompt, ignoreString);
  strcat(prompt, KCYN);
  strcat(prompt, allowString);
  strcat(prompt, "jsh ");
  strcat(prompt, ignoreString);
  strcat(prompt, KGRN);
  strcat(prompt, allowString);
  strcat(prompt, getcwd(cwd, 100));
  strcat(prompt, ignoreString);
  strcat(prompt, KCYN);
  strcat(prompt, allowString);
  strcat(prompt, " $ ");
  strcat(prompt, ignoreString);
  strcat(prompt, KNRM);
  strcat(prompt, allowString);
  if (cwd != NULL) {
    free(cwd);
  }
  return prompt;
}

static void printRainbow(char* line) {
  static int size = 7;
  static int colorIndex = 0;
  static char* colors[] = {
    KRED, KGRN, KYEL, KBLU, KMAG, KCYN, KWHT
  };
  int i = 0;
  while (line[i]) {
    fprintf(stderr, "%s%c", colors[colorIndex % size], line[i]);
    ++i;
    ++colorIndex;
  }
  fprintf(stderr, "%s", KNRM);
}
