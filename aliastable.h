/*
 * aliastable.h
 *
 *  Created on: Mar 28, 2015
 *      Author: joe
 */

#ifndef ALIASTABLE_H_
#define ALIASTABLE_H_

void mapAlias(char* name, char* toWord);
void unmapAlias(char* name);
void printAliasTable();
int checkAliasExists(char* name);

#endif /* ALIASTABLE_H_ */
