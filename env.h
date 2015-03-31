/*
 * printenv.h
 *
 *  Created on: Mar 29, 2015
 *      Author: joe
 */

#ifndef ENV_H_
#define ENV_H_

void printEnv();
void setEnv(const char* variable, const char* word);
void unsetEnv(const char* variable);
char* expandVariables(char* word);

#endif /* ENV_H_ */
