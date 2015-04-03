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
  if (checkAliasExists(name)) {
    return;
  }
  alias_keys[alias_count] = strdup(name);
  alias_values[alias_count] = strdup(toWord);
  ++alias_count;
}

int checkAliasExists(char* name) {
  int i;
  for (i = 0; i < alias_count; ++i) {
    if (strcmp(name, alias_keys[i]) == 0)
      return TRUE;
  }
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
  for (i = 0; i < alias_count; ++i)
    printf("%s='%s'\n", alias_keys[i], alias_values[i]);
}
