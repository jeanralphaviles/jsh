/*
 * utils.c
 *
 *  Created on: Mar 28, 2015
 *      Author: joe
 */

#include <glob.h>
#include <pwd.h>
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
      //if (isEscapeCharacter(cur)) {
        copy[newLength] = cur;
        newLength++;
      /*} else {
        fprintf(stderr, "Unrecognized escape sequence \\%d\n", cur);
      }*/
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
  char escapeCharacters[3] = {'\\', ' ', '\''};
  if (isMetaCharacter(character)) {
    return TRUE;
  }
  int i;
  for (i = 0; i < 3; ++i) {
	  if (character == escapeCharacters[i]) {
		  return TRUE;
	  }
  }
  return FALSE;
}

char** wildcardMatch(char* cmd_name, char** argv, char* token_sep) {
  glob_t globbuf;
  glob(cmd_name, GLOB_NOCHECK | GLOB_TILDE, NULL, &globbuf);
  int i = 1;
  while (argv[i]) {
    if (token_sep == NULL) {
      argv[i] = tildeExpand(argv[i]);
      glob(argv[i++], GLOB_NOCHECK | GLOB_APPEND, NULL, &globbuf);
    } else {
      // Split each argument based on given token, only tilde expand
      char** tokens = (char**)malloc(sizeof(char*)*100);
      char* token = strtok(argv[i++], token_sep);
      int j = 0;
      while (token) {
        token = tildeExpand(token);
        tokens[j++] = token;
        token = strtok(NULL, token_sep);
      }
      tokens[j] = NULL;
      // Merge all tokens into one string
      int totalTokenLength = 0;
      j = 0;
      while (tokens[j]) {
        totalTokenLength += strlen(tokens[j++]);
      }
      char* merged = (char*)malloc(totalTokenLength + 1 + (j - 1)); /* j-1 is for separators */
      memset(merged, 0, totalTokenLength + 1);
      j = 0;
      while (tokens[j]) {
        strcat(merged, tokens[j++]);
        strcat(merged, token_sep);
      }
      merged[totalTokenLength + (j - 1)] = '\0';
      // Add merged to the rest of the glob
      glob(merged, GLOB_NOCHECK | GLOB_APPEND, NULL, &globbuf);
      free(tokens);
    }
  }
  return globbuf.gl_pathv;
}

// ~ or ~/ expands to current user's $HOME variable
// ~username or ~username/ expands to username's $HOME
char* tildeExpand(char* path_name) {
  // Paths must start with ~ to be tilde expanded
  if (path_name == NULL || path_name[0] != '~') {
    return path_name;
  }

  char* firstSlash; /* Pointer to first slash in path_name */
  if ((firstSlash = strchr(path_name, '/')) == NULL) {
    // Did not find a slash, set firstSlash to end of string
    firstSlash = path_name + strlen(path_name) + 1;
  }
  int usernameLength = firstSlash - path_name - 1;
  char* username = (char*)malloc(usernameLength + 1);
  username = strncpy(username, path_name + 1, usernameLength);
  username[usernameLength] = '\0';

  char* homeDir;
  if (strlen(username) == 0) {
    // If username was just ~, get current user's $HOME
    homeDir = getenv("HOME");
  } else {
    // Else, get username's $HOME
    struct passwd* userInfo = getpwnam(username);
    if (userInfo == NULL) {
      // System, doesn't recognize user, don't expand
      return path_name;
    }
    homeDir = userInfo->pw_dir;
  }

  // Now replace ~ with homeDir
  char* restOfString = firstSlash; /* String after tilde expansion */
  int expandedStringSize = strlen(homeDir) + strlen(restOfString);
  char* expandedString = (char*)malloc(expandedStringSize +  1);
  memset(expandedString, 0, expandedStringSize + 1);
  strcat(expandedString, homeDir);
  strcat(expandedString, restOfString);

  return expandedString;
}

