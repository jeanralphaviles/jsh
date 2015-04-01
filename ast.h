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
};

struct AstSingleCommand {
  char* cmd_name;
  struct queue* args;
  char* io_in;
  char* io_out;
};

// All create functions assume owenership of arguments
struct AstRoot* createAstRoot();
struct AstPipeSequence* createAstPipeSequence();
struct AstSingleCommand* createAstSingleCommand(char* cmd_name, char* io_in, char* io_out);

// Member Functions
// --AstSingleCommand--
void setIoIn(struct AstSingleCommand*, char*);
void setIoOut(struct AstSingleCommand*, char*);
void addArgs(struct AstSingleCommand*, char*);
// --AstPipeSequence--
void addCommand(struct AstPipeSequence*, struct AstSingleCommand*);
// --AstRoot--
void addPipeSequence(struct AstRoot*, struct AstPipeSequence*);
void addPipeSequenceWithSeparator(struct AstRoot*, struct AstPipeSequence*, int); // 1 => &&, 2 => ||

// Execute
int executeAstRoot(struct AstRoot*);
int executePipeSequence(struct AstPipeSequence*);
int executeCommand(struct AstSingleCommand*);

// Utility
bool fileExists(char*);

#endif
