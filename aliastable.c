/*
 * aliastable.c
 *
 *  Created on: Mar 28, 2015
 *      Author: joe
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "aliastable.h"
#include "defines.h"

/* How to make dynamic arrays in C, I'm not sure.
 * For now I hope they don't make 1024 alias mappings. */

char* alias_keys[MAX_LENGTH];
char* alias_values[MAX_LENGTH];
static int alias_count = 0;

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
    fprintf(stderr, "Alias: %s Already Exists!\n", name);
    return;
  }
  alias_keys[alias_count] = name;
  alias_values[alias_count] = toWord;
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
  for (i = index; i < alias_count; ++i) {
    // Shift every entry alias to the left (inefficient!) :/
    // Thus overwriting the deleted entry.
    alias_keys[i] = alias_keys[i + 1];
    alias_values[i] = alias_values[i + 1];
  }
  // Dealloc the last entry.
  free(alias_keys[alias_count - 1]);
  free(alias_values[alias_count - 1]);
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
  fprintf(stderr, "Successfully Unmapped Alias: %s\n", name);
}

void printAliasTable() {
  int i;
  for (i = 0; i < alias_count; ++i)
    printf("%s=%s\n", alias_keys[i], alias_values[i]);
}
