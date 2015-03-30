/*
 * utils.c
 *
 *  Created on: Mar 28, 2015
 *      Author: joe
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "defines.h"

void fixPath(char* path) {
	// If the path is an Absolute Path, no fixing to do.
	if (path[0] == '/') return;

	char* wd = (char*)malloc(MAX_LENGTH);

	getcwd(wd, MAX_LENGTH);

	strcat(wd, "/");
	strcat(wd, path);
	path = wd;
}
