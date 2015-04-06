/*
 * utils.h
 *
 *  Created on: Mar 28, 2015
 *      Author: joe
 */

#ifndef UTILS_H_
#define UTILS_H_

void fixPath(char* path);
int isAbsolutePath(char* path);
char* resolveEscapeSequences(char* line);
int isMetaCharacter(char character);
int isEscapeCharacter(char character);
char** wildcardMatch(char*, char**, bool*, char*);
char* tildeExpand(char*);

#endif /* UTILS_H_ */
