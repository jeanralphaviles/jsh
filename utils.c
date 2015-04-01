/*
 * utils.c
 *
 *  Created on: Mar 28, 2015
 *      Author: joe
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "defines.h"
#include "utils.h"

const char* currTermColor = KNRM;

void fixPath(char* path) {
  // If the path is an Absolute Path, no fixing to do.
  if (isAbsolutePath(path)) return;

  char* wd = (char*)malloc(MAX_LENGTH);

  getcwd(wd, MAX_LENGTH);

  strcat(wd, "/");
  strcat(wd, path);
  path = wd;
}

int isAbsolutePath(char* path) {
  if (path[0] == '/') return TRUE;
  else return FALSE;
}

const char* setTermColor(FILE* stream, const char* newColor) {
  const char* oldColor = currTermColor;
  currTermColor = newColor;
  fprintf(stream, "%s", newColor);
  return oldColor;
}
