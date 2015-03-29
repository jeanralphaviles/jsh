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
		printf("Attempting to 'cd' to: %s\n", dest);
		int val = chdir(dest);
		return val;
	}
	else if (strcmp(cmd, "alias") == 0) {
		if (argc > 2) {
			char* name = argv[2];
			char* word = argv[3];
			checkAliasExists(name);
		}
	}
	return 1;
}
