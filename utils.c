/*
 * utils.c
 *
 *  Created on: Mar 28, 2015
 *      Author: joe
 */

#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "defines.h"
#include "utils.h"

const char* currTermColor = KNRM;

void fixPath(char* path) {
  // If the path is an Absolute Path, no fixing to do.
  if (isAbsolutePath(path)) {
    return;
  }

  char* wd = (char*)malloc(MAX_LENGTH);

  getcwd(wd, MAX_LENGTH);

  strcat(wd, "/");
  strcat(wd, path);
  path = wd;
}

int isAbsolutePath(char* path) {
  if (path[0] == '/') {
    return TRUE;
  } else {
    return FALSE;
  }
}

const char* setTermColor(FILE* stream, const char* newColor) {
  const char* oldColor = currTermColor;
  currTermColor = newColor;
  fprintf(stream, "%s", newColor);
  return oldColor;
}

char* resolveEscapeSequences(char* line) {
  int length = strlen(line);
  char* copy = (char*) malloc(length + 1);
  int i;
  int foundEscape = FALSE;
  int newLength = 0;
  for (i = 0; i < length; ++i) {
    char cur = line[i];
    if (foundEscape) {
      if (isEscapeCharacter(cur)) {
        copy[newLength] = cur;
        newLength++;
      } else {
        fprintf(stderr, "Unrecognized escape sequence \\%d\n", cur);
      }
      foundEscape = FALSE;
      continue;
    }

    // Found an escape sequence
    if (cur == '\\') {
      foundEscape = TRUE;
    } else {
      copy[newLength] = cur;
      newLength++;
    }
  }
  copy[newLength] = '\0';
  free(line);
  return copy;
}

int isMetaCharacter(char character) {
  char metaCharacters[5] = { '&', '|', '"', '<', '>' };
  int i;
  for (i = 0; i < 5; ++i) {
    if (character == metaCharacters[i]) {
      return TRUE;
    }
  }
  return FALSE;
}

int isEscapeCharacter(char character) {
  char backslashChar = '\\';
  if (isMetaCharacter(character)) {
    return TRUE;
  }
  return character == backslashChar;
}

char** wildcardMatch(char* cmd_name, char** argv, char* token_sep) {
  glob_t globbuf;
  glob(cmd_name, GLOB_NOCHECK | GLOB_TILDE, NULL, &globbuf);
  int i = 1;
  while (argv[i]) {
    if (token_sep == NULL) {
      glob(argv[i++], GLOB_NOCHECK | GLOB_APPEND | GLOB_TILDE, NULL, &globbuf);
    } else {
      // Split each argument based on given token
      char* token = strtok(argv[i++], token_sep);
      while (token) {
        printf("token %s\n", token);
        glob(token, GLOB_NOCHECK | GLOB_APPEND | GLOB_TILDE | GLOB_ONLYDIR, NULL, &globbuf);
        token = strtok(NULL, token_sep);
      }
    }
  }
  i = 0;
  while (globbuf.gl_pathv[i]) {
    printf("wild carded %s\n", globbuf.gl_pathv[i++]);
  }
  return globbuf.gl_pathv;
}

