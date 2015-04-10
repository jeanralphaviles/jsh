/*
 * printenv.c
 *
 *  Created on: Mar 29, 2015
 *      Author: joe
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void printEnvSingleVar(char* variable) {
  char** env;
  char* envLinePtr;

  for (env = environ; *env; ++env) {
    envLinePtr = *env;
    while (*envLinePtr != '\0' && *variable != '\0' && *envLinePtr == *variable) {
      ++envLinePtr;
      ++variable;

      if (*envLinePtr == '=') {
        printf("%s\n", envLinePtr + 1);
        return;
      }
    }
  }
}

void setEnv(const char* variable, const char* word) {
  int val = setenv(variable, word, 1);
}

void unsetEnv(const char* variable) {
  unsetenv(variable);
  if (errno == EINVAL) {
    const char* oldColor = setTermColor(stderr, KRED);
    fprintf(stderr, "unsetenv: failed environment variable %s not found \n", variable);
    setTermColor(stderr, oldColor);
  }
}

char* expandVariables(char* word) {
  char* copy = (char*) calloc(MAX_LENGTH,1);
  int copyPtr = 0;
  int i;

  for (i = 0; i < MAX_LENGTH && word[i] != '\0'; ++i) {
    if (word[i] == '$' && word[i + 1] != '\0' && word[i + 1] == '{') {
      char* envVar = (char*) calloc(MAX_LENGTH, 1);
      int envVarPtr = 0;
      i += 2;
      while (i < MAX_LENGTH && word[i] != '\0' && word[i] != '}') {
        envVar[envVarPtr] = word[i];
        ++envVarPtr;
        ++i;
        if (word[i] == '}') {
          envVar[envVarPtr] = '\0';
          char* temp = (char*) calloc(MAX_LENGTH, 1);
          if (getenv(envVar) != NULL) {
            sprintf(temp, "%s", getenv(envVar));
			int j;
			for (j = 0; j < strlen(temp); ++j) {
			    copy[copyPtr] = temp[j];
			    ++copyPtr;
			}
			free(temp);
          }

          // No env var found, return ${VAR}
          else {
            strcat(temp, "${");
            strcat(temp, envVar);
            strcat(temp, "}");
          }
          free(envVar);
          envVarPtr = 0;
        }
      }
      // ++i;
    }
    else {
      copy[copyPtr] = word[i];
      ++copyPtr;
    }
  }
  free(word);
  copy[copyPtr] = '\0';
  return copy;
}
