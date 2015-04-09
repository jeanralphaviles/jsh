/*
 * builtins.c
 *
 *  Created on: Mar 15, 2015
 *      Author: Joseph Liccini
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "aliastable.h"
#include "builtins.h"
#include "defines.h"
#include "env.h"
#include "utils.h"

int executeBuiltinCommand(char* cmd, int argc, char** argv) {
  int status = SUCCESS;
  if (strcmp(cmd, "cd") == 0) {
    char* holdThis = (char*)malloc(MAX_LENGTH);
    char* dest = holdThis;
    if (argc == 2) {
      dest = argv[1];
      fixPath(dest);
    } else if (argc > 2) {
      const char* oldColor = setTermColor(stderr, KRED);
      fprintf(stderr, "cd: too many arguments\n");
      setTermColor(stderr, oldColor);
      status = ERROR;
    } else {
      strncpy(dest, getenv("HOME"), MAX_LENGTH);
    }
    trimGarbage(dest);
    if (chdir(dest) == -1) {
      const char* oldColor = setTermColor(stderr, KRED);
      if (errno == EACCES) {
        fprintf(stderr, "cd: permission denied\n");
      } else if (errno == ENOENT) {
        fprintf(stderr, "cd: directory does not exist\n");
      }
      setTermColor(stderr, oldColor);
    }
    free(holdThis);
  } else if (strcmp(cmd, "alias") == 0) {
    if (argc == 3) {
      char* name = argv[1];
      char* word = argv[2];
      mapAlias(name, word);
    } else if (argc == 1) {
      printAliasTable();
    } else {
      const char* oldColor = setTermColor(stderr, KRED);
      fprintf(stderr, "alias: need 3 arguments, got %d\n", argc);
      setTermColor(stderr, oldColor);
      status = ERROR;
    }
  } else if (strcmp(cmd, "unalias") == 0) {
    if (argc == 2) {
      char* name = argv[1];
      unmapAlias(name);
    } else {
      const char* oldColor = setTermColor(stderr, KRED);
      fprintf(stderr, "unalias: need 2 arguments, got %d\n", argc);
      setTermColor(stderr, oldColor);
      status = ERROR;
    }
  } else if (strcmp(cmd, "printenv") == 0) {
    if (argc < 2) {
      printEnv();
    } else {
      const char* oldColor = setTermColor(stderr, KRED);
      fprintf(stderr, "printenv: too many arguments");
      setTermColor(stderr, oldColor);
      status = ERROR;
    }
  } else if (strcmp(cmd, "setenv") == 0) {
    if (argc == 3) {
      char* variable = argv[1];
      char* word = argv[2];
      setEnv(variable, word);
    } else {
      const char* oldColor = setTermColor(stderr, KRED);
      fprintf(stderr, "setenv: need 3 arguments, got %d\n", argc);
      setTermColor(stderr, oldColor);
      status = ERROR;
    }
  } else if (strcmp(cmd, "unsetenv") == 0) {
    if (argc == 2) {
      char* variable = argv[1];
      unsetEnv(variable);
    } else {
      const char* oldColor = setTermColor(stderr, KRED);
      fprintf(stderr, "unsetenv: need 2 arguments, got %d\n", argc);
      setTermColor(stderr, oldColor);
      status = ERROR;
    }
  } else if (strcmp(cmd, "bye") == 0) {
    exit(EXIT_SUCCESS);
  } else {
    status = ERROR;
  }
  return status;
}

bool isBuiltinNormalCommand(struct AstSingleCommand* command) {
  char* cmd = command->cmd_name;
  if (strcmp(cmd, "cd") == 0) return TRUE;
  else if (strcmp(cmd, "alias") == 0 && size(command->args) > 1) return TRUE;
  else if (strcmp(cmd, "unalias") == 0) return TRUE;
  else if (strcmp(cmd, "printenv") == 0 && size(command->args) > 1) return TRUE;
  else if (strcmp(cmd, "setenv") == 0) return TRUE;
  else if (strcmp(cmd, "unsetenv") == 0) return TRUE;
  else if (strcmp(cmd, "bye") == 0) return TRUE;
  return FALSE;
}

bool isBuiltinPipeCommand(struct AstSingleCommand* command) {
  char* cmd = command->cmd_name;
  if (strcmp(cmd, "alias") == 0 && size(command->args) == 1) return TRUE;
  else if (strcmp(cmd, "printenv") == 0 && size(command->args) == 1) return TRUE;
  return FALSE;
}

void trimGarbage(char* line) {
  int i = 0;
  while(line[i]) {
    if (line[i] > 127) {
      line[i] = '\0';
    }
    ++i;
  }
}
