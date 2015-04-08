/*
 * aliastable.h
 *
 *  Created on: Mar 28, 2015
 *      Author: joe
 */

#ifndef ALIASTABLE_H_
#define ALIASTABLE_H_

#include "defines.h"

void mapAlias(char* name, char* toWord);
void unmapAlias(char* name);
void printAliasTable();
char* getAlias(char* key);
bool checkAliasExists(char* name);
bool isInfiniteAlias(char* name, char* word);
void aliasSub(char*);
bool isCommandSeparator(char*);
bool containsAlias(char*);

#endif /* ALIASTABLE_H_ */
