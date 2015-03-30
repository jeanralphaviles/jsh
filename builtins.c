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

#include "builtins.h"
#include "defines.h"
#include "utils.h"

int checkBuiltInCommand(char* cmd, int argc, char** argv) {
  if (strcmp(cmd, "cd") == 0) {
    char* dest = (char*)malloc(MAX_LENGTH);
    if (argc > 1) {
      //free(dest);
      dest = argv[1];
      fixPath(dest);
    } else {
     sprintf(dest, "%s", getenv("HOME"));
    }
    fprintf(stderr, "Attempting to 'cd' to: %s\n", dest);
    int val = chdir(dest);
    return val;
  }
  return 1;
}
