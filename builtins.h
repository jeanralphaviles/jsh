/*
 * builtins.h
 *
 *  Created on: Mar 15, 2015
 *      Author: joe
 */

#ifndef BUILTINS_H_
#define BUILTINS_H_

#include "ast.h"

int executeBuiltinCommand(char* cmd, int argc, char** argv);
bool isBuiltinNormalCommand(struct AstSingleCommand*);
bool isBuiltinPipeCommand(struct AstSingleCommand*);

#endif /* BUILTINS_H_ */
