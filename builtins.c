/*
 * builtins.c
 *
 *  Created on: Mar 15, 2015
 *      Author: Joseph Liccini
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "aliastable.h"
#include "builtins.h"
#include "defines.h"
#include "env.h"
#include "utils.h"

void executeBuiltinCommand(char* cmd, int argc, char** argv) {
  if (strcmp(cmd, "cd") == 0) {
    char* dest = (char*)malloc(MAX_LENGTH);
    if (argc == 2) {
      dest = argv[1];
      fixPath(dest);
    } else if (argc > 2) {
      const char* oldColor = setTermColor(stderr, KRED);
      fprintf(stderr, "cd: too many arguments\n");
      setTermColor(stderr, oldColor);
    } else {
      sprintf(dest, "%s", getenv("HOME"));
    }
    int val = chdir(dest);
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
    }
  } else if (strcmp(cmd, "unalias") == 0) {
    if (argc == 2) {
      char* name = argv[1];
      unmapAlias(name);
    } else {
      const char* oldColor = setTermColor(stderr, KRED);
      fprintf(stderr, "unalias: need 2 arguments, got %d\n", argc);
      setTermColor(stderr, oldColor);
    }
  } else if (strcmp(cmd, "printenv") == 0) {
    if (argc < 2) {
      printEnv();
    } else {
      const char* oldColor = setTermColor(stderr, KRED);
      fprintf(stderr, "printenv: need 3 arguments, got %d\n", argc);
      setTermColor(stderr, oldColor);
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
    }
  } else if (strcmp(cmd, "unsetenv") == 0) {
    if (argc == 2) {
      char* variable = argv[1];
      unsetEnv(variable);
    } else {
      const char* oldColor = setTermColor(stderr, KRED);
      fprintf(stderr, "unsetenv: need 2 arguments, got %d\n", argc);
      setTermColor(stderr, oldColor);
    }
  } else if (strcmp(cmd, "bye") == 0) {
    exit(EXIT_SUCCESS);
  }
}

int isBuiltinCommand(char* cmd) {
  if (strcmp(cmd, "cd") == 0) return TRUE;
  else if (strcmp(cmd, "alias") == 0) return TRUE;
  else if (strcmp(cmd, "unalias") == 0) return TRUE;
  else if (strcmp(cmd, "printenv") == 0) return TRUE;
  else if (strcmp(cmd, "setenv") == 0) return TRUE;
  else if (strcmp(cmd, "unsetenv") == 0) return TRUE;
  else if (strcmp(cmd, "bye") == 0) return TRUE;
  return FALSE;
}
