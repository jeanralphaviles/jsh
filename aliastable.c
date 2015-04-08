/*
 * aliastable.c
 *
 *  Created on: Mar 28, 2015
 *      Author: joe
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aliastable.h"
#include "defines.h"
#include "utils.h"

/* How to make dynamic arrays in C, I'm not sure.
 * For now I hope they don't make 1024 alias mappings. */

char* alias_keys[MAX_LENGTH];
char* alias_values[MAX_LENGTH];
int alias_count = 0;

char* getAlias(char* key) {
  int i;
  for (i = 0; i < alias_count; ++i) {
    if (strcmp(key, alias_keys[i]) == 0)
      return alias_values[i];
  }
  return key;
}

void mapAlias(char* name, char* toWord) {
  const char* old_color = setTermColor(stderr, KRED);
  if (checkAliasExists(name)) {
    fprintf(stderr, "alias: alias already exists\n");
  } else if (strcmp(name, toWord) == 0) {
    fprintf(stderr, "alias: cannot alias word to itself\n");
  } else if (isInfiniteAlias(name, toWord)) {
    fprintf(stderr, "alias: refusing infinite alias\n");
  } else if (strcmp(name, "alias") == 0) {
    fprintf(stderr, "alias: cannot alias alias\n");
  } else if (strcmp(name, "unalias") == 0) {
    fprintf(stderr, "alias: cannot alias unalias\n");
  } else {
    alias_keys[alias_count] = strdup(name);
    alias_values[alias_count] = strdup(toWord);
    ++alias_count;
  }
  setTermColor(stderr, old_color);
}

bool checkAliasExists(char* name) {
  int i;
  for (i = 0; i < alias_count; ++i) {
    if (strcmp(name, alias_keys[i]) == 0) {
      return TRUE;
    }
  }
  return FALSE;
}

bool isInfiniteAlias(char* name, char* word) {
  char* line = (char*)calloc(MAX_LENGTH, 1);
  strcpy(line, word);
  while (containsAlias(line)) {
    bool commandNext = TRUE;
    char* substituted = (char*)calloc(MAX_LENGTH, 1);
    char* token = strtok(line, " ");
    while (token) {
      if (commandNext == TRUE) {
        // If token is an alias for word or is equal to word itself.
        if ((checkAliasExists(token) && strcmp(name, getAlias(token)) == 0) ||
            strcmp(name, token) == 0) {
          free(line);
          free(substituted);
          return TRUE;
        }
        strcat(substituted, getAlias(token));
        commandNext = FALSE;
      } else {
        strcat(substituted, token);
        commandNext = isCommandSeparator(token);
      }
      strcat(substituted, " ");
      token = strtok(NULL, " ");
    }
    strncpy(line, substituted, 1023);
    line[MAX_LENGTH] = '\0';
    free(substituted);
  }
  free(line);
  return FALSE;
}

void deleteEntry(int index) {
  int i;
  char* dkey = alias_keys[index];
  char* dvalue = alias_values[index];
  for (i = index; i < alias_count; ++i) {
    // Shift every entry alias to the left (inefficient!) :/
    // Thus overwriting the deleted entry.
    alias_keys[i] = alias_keys[i + 1];
    alias_values[i] = alias_values[i + 1];
  }
  // Dealloc the removed entry.
  free(dkey);
  free(dvalue);
  --alias_count;
}

void unmapAlias(char* name) {
  if (!(checkAliasExists(name))) {
    fprintf(stderr, "Alias: %s does not exist!\n", name);
    return;
  }
  int i, target_index;
  for (i = 0; i < alias_count; ++i) {
    if (strcmp(name, alias_keys[i]) == 0) {
      target_index = i;
      break;
    }
  }
  deleteEntry(target_index);
}

void printAliasTable() {
  int i;
  for (i = 0; i < alias_count; ++i) {
    printf("%s='%s'\n", alias_keys[i], alias_values[i]);
  }
}

void aliasSub(char* line) {
  // Strip newline
  line[strlen(line) - 1] = '\0';
  while (containsAlias(line)) {
    bool commandNext = TRUE;
    char* substituted = (char*)malloc(MAX_LENGTH);
    substituted[0] = '\0';
    char* word = strtok(line, " ");
    while (word) {
      if (commandNext == TRUE && checkAliasExists(word) == TRUE) {
        strcat(substituted, getAlias(word));
        commandNext = FALSE;
      } else {
        strcat(substituted, word);
        commandNext = isCommandSeparator(word);
      }
      strcat(substituted, " ");
      word = strtok(NULL, " ");
    }
    strncpy(line, substituted, 1023);
    line[MAX_LENGTH] = '\0';
    free(substituted);
  }
  // Re-add newline
  strcpy(line + strlen(line), "\n\0");
}

bool isCommandSeparator(char* word) {
  if (strcmp(word, "&") == 0) return TRUE;
  else if (strcmp(word, "&&") == 0) return TRUE;
  else if (strcmp(word, "|") == 0) return TRUE;
  else if (strcmp(word, "||") == 0) return TRUE;
  else return FALSE;
}

bool containsAlias(char* line) {
  char* savedLine = (char*)malloc(strlen(line) + 1);
  strcpy(savedLine, line);
  bool commandNext = TRUE;
  char* word = strtok(line, " ");
  while (word != NULL) {
    if (commandNext == TRUE && checkAliasExists(word) == TRUE) {
      strcpy(line, savedLine);
      free(savedLine);
      return TRUE;
    } else if (isCommandSeparator(word)) {
      commandNext = TRUE;
    } else {
      commandNext = FALSE;
    }
    word = strtok(NULL, " ");
  }
  strcpy(line, savedLine);
  free(savedLine);
  return FALSE;
}
