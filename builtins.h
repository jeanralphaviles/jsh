/*
 * builtins.h
 *
 *  Created on: Mar 15, 2015
 *      Author: joe
 */

#ifndef BUILTINS_H_
#define BUILTINS_H_

int checkBuiltInCommand(char* cmd, int argc, char** argv);
void fixPath(char* path);

#endif /* BUILTINS_H_ */
