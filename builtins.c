/*
 * builtins.c
 *
 *  Created on: Mar 15, 2015
 *      Author: Joseph Liccini
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "builtins.h"
#include "defines.h"
#include "utils.h"
#include "aliastable.h"

int checkBuiltInCommand(char* cmd, int argc, char** argv) {
	if (strcmp(cmd, "cd") == 0) {
		char* dest = (char*)malloc(MAX_LENGTH);
		if (argc > 1) {
			//free(dest);
			dest = argv[1];
			fixPath(dest);
		}
		else
			sprintf(dest, "%s", getenv("HOME"));
		int val = chdir(dest);
		printf("'cd' command exited with code: %d\n", val);
		return TRUE;
	}
	else if (strcmp(cmd, "alias") == 0) {
		if (argc > 2) {
			char* name = argv[1];
			char* word = argv[2];
			mapAlias(name, word);
		}
		else if (argc == 1) {
			printAliasTable();
		}
		return TRUE;
	}
	else if (strcmp(cmd, "unalias") == 0) {
		if (argc > 1) {
			char* name = argv[1];
			unmapAlias(name);
		}
		return TRUE;
	}
	return FALSE;
}
