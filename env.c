/*
 * printenv.c
 *
 *  Created on: Mar 29, 2015
 *      Author: joe
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "builtins.h"
#include "defines.h"
#include "env.h"
#include "utils.h"

extern char** environ;

void printEnv() {
	char** env;
	for(env = environ; *env; ++env) {
		printf("%s\n", *env);
	}
}

void setEnv(const char* variable, const char* word) {
	int val = setenv(variable, word, 1);
	printf("Output of setEnv: %d\n", val);
}

void unsetEnv(const char* variable) {
	int val = unsetenv(variable);
	printf("Output of unsetEnv: %d\n", val);
}
