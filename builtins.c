#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "builtins.h"
#include "defines.h"
/*
 * builtins.c
 *
 *  Created on: Mar 15, 2015
 *      Author: Joseph Liccini
 */


int checkBuiltInCommand(char* cmd, int argc, char** argv) {
	if (strcmp(cmd, "cd") == 0) {
		const char* dest = (char*)malloc(MAX_LENGTH);
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
	return 1;
}

void fixPath(char* path) {
	// If the path is an Absolute Path, no fixing to do.
	if (path[0] == '/') return;

	char* wd = (char*)malloc(MAX_LENGTH);

	getcwd(wd, MAX_LENGTH);

	strcat(wd, "/");
	strcat(wd, path);
	path = wd;
}
