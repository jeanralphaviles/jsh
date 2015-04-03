#ifndef AST_H
#define AST_H

#include "defines.h"
#include "queue.h"

struct AstRoot {
  bool async;
  struct queue *pipe_sequences;
  struct queue *sequence_separators;
};

struct AstPipeSequence {
  struct queue *commands;
  char* io_in;
  char* io_out;
  char* io_err;
  bool err2out;
  bool append_out;
};

struct AstSingleCommand {
  char* cmd_name;
  struct queue* args;
};

// All create functions assume owenership of arguments
struct AstRoot* createAstRoot();
struct AstPipeSequence* createAstPipeSequence();
struct AstSingleCommand* createAstSingleCommand(char* cmd_name);

// Member Functions
// --AstSingleCommand--
void addArgs(struct AstSingleCommand*, char*);
// --AstPipeSequence--
void addCommand(struct AstPipeSequence*, struct AstSingleCommand*);
void setIoIn(struct AstPipeSequence*, char*);
void setIoOut(struct AstPipeSequence*, char*, bool);
void setIoErr(struct AstPipeSequence*, char*);
// --AstRoot--
void addPipeSequence(struct AstRoot*, struct AstPipeSequence*);
void addPipeSequenceWithSeparator(struct AstRoot*, struct AstPipeSequence*, int); // 1 => &&, 2 => ||

// Execute
int executeAstRoot(struct AstRoot*);
int executePipeSequence(struct AstPipeSequence*);
void executeCommand(struct AstSingleCommand*);

// Utility
bool fileExists(char*);
char** getArgs(struct AstSingleCommand*);

#endif
