/*
 * builtins.h
 *
 *  Created on: Mar 15, 2015
 *      Author: joe
 */

#ifndef BUILTINS_H_
#define BUILTINS_H_

void executeBuiltinCommand(char* cmd, int argc, char** argv);
int isBuiltinCommand(char* cmd);

#endif /* BUILTINS_H_ */
