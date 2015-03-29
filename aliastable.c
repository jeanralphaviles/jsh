/*
 * aliastable.c
 *
 *  Created on: Mar 28, 2015
 *      Author: joe
 */
#include "aliastable.h"
#include "defines.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* How to make dynamic arrays in C, I'm not sure.
 * For now I hope they don't make 1024 alias mappings.
 * Is there even a HashMap? */

char* alias_keys[MAX_LENGTH];
char* alias_values[MAX_LENGTH];
static int alias_count = 0;

void mapAlias(char* name, char* toWord) {
	if (checkAliasExists(name) == 0) {
		printf("Alias: %s Already Exists!\n", name);
		return;
	}
	alias_keys[alias_count] = name;
	alias_values[alias_count] = toWord;
	++alias_count;
	printf("Mapped Alias: %s to %s\n", name, toWord);
	printf("Number of Aliases: %d\n", alias_count);
}

int checkAliasExists(char* name) {
	int i;
	for (i = 0; i < alias_count; ++i) {
		if (strcmp(name, alias_keys[i]) == 0) {
			return 0;
		}
	}
	return 1;
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
	if (!(checkAliasExists(name) == 0)) {
		printf("Alias: %s does not exist!", name);
		return;
	}
	int target_index = -1;
	int i;
	for (i = 0; i < alias_count; ++i) {
		if (strcmp(name, alias_keys[i]) == 0) {
			target_index = i;
			break;
		}
	}
	deleteEntry(target_index);
}
