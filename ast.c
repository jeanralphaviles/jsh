#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "ast.h"
#include "defines.h"
#include "builtins.h"
#include "queue.h"

struct AstRoot* createAstRoot() {
  struct AstRoot* ast_root = (struct AstRoot*)malloc(sizeof(struct AstRoot));

  ast_root->async = FALSE;
  ast_root->pipe_sequences = createQueue();
  ast_root->sequence_separators = createQueue();
  return ast_root;
}

struct AstPipeSequence* createAstPipeSequence() {
  struct AstPipeSequence* ast_pipe_sequence = (struct AstPipeSequence*)malloc(sizeof(struct AstPipeSequence));

  ast_pipe_sequence->commands = createQueue();
  return ast_pipe_sequence;
}

struct AstSingleCommand* createAstSingleCommand(char* cmd_name, char* io_in, char* io_out) {
  struct AstSingleCommand* ast_single_command = (struct AstSingleCommand*)malloc(sizeof(struct AstSingleCommand));

  ast_single_command->cmd_name = (char*)malloc(strlen(cmd_name) + 1);
  strcpy(ast_single_command->cmd_name, cmd_name);
  ast_single_command->args = createQueue();
  enqueue(ast_single_command->args, cmd_name);
  ast_single_command->io_in = io_in;
  ast_single_command->io_out = io_out;
  return ast_single_command;
}

// Member Functions
// --AstSingleCommand--
void setIoIn(struct AstSingleCommand* ast, char* in) {
  ast->io_in = strdup(in);
}

void setIoOut(struct AstSingleCommand* ast, char* out) {
  ast->io_out = strdup(out);
}

void addArgs(struct AstSingleCommand* ast, char* args) {
  if (ast->args == NULL) {
    ast->args = createQueue();
  }
  enqueue(ast->args, args);
}

// --AstPipeSequence--
void addCommand(struct AstPipeSequence* pipe_sequence, struct AstSingleCommand* command) {
  if (pipe_sequence->commands == NULL) {
    pipe_sequence->commands = createQueue();
  }
  enqueue(pipe_sequence->commands, command);
}

// --AstRoot--
void addPipeSequence(struct AstRoot* root, struct AstPipeSequence* pipe_sequence) {
  if (root->pipe_sequences == NULL) {
    root->pipe_sequences =createQueue();
  }
  enqueue(root->pipe_sequences, pipe_sequence);
}

void addPipeSequenceWithSeparator(struct AstRoot* root, struct AstPipeSequence* pipe_sequence, int separator) { // 0 => &&, 1 => ||
  if (root->sequence_separators == NULL) {
    root->sequence_separators = createQueue();
  }
  if (root->pipe_sequences == NULL || size(root->pipe_sequences) == 0 || size(root->pipe_sequences) != size(root->sequence_separators) + 1) {
    fprintf(stderr, "PipeSequences and Separators are messed up\n");
  }
  enqueue(root->pipe_sequences, pipe_sequence);
  enqueue(root->sequence_separators, (void*)separator);
}

// Execute
int executeAstRoot(struct AstRoot* root) {
  if (root == NULL || size(root->pipe_sequences) == 0) {
    return ERR_NOT_FOUND;
  }
  if (size(root->pipe_sequences) == 1) {
    return executePipeSequence((struct AstPipeSequence*)front(root->pipe_sequences));
  } else {
    // TODO
  }
}

int executePipeSequence(struct AstPipeSequence* pipe_sequence) {
  if (size(pipe_sequence->commands) == 0) {
    return ERR_NOT_FOUND;
  } else if (size(pipe_sequence->commands) == 1) { // Only One command, no pipes
    return executeCommand((struct AstSingleCommand*)front(pipe_sequence->commands));
  } else {
    // TODO
  }
}

int executeCommand(struct AstSingleCommand* command) {
  int process; // pid of child
  int status; // status of child
  char* temp = NULL;
  char* PATH = (char*)malloc(MAX_LENGTH);
  char* cmd_name = command->cmd_name;
  char** argv = (char**)malloc(size(command->args));
  int i = 0;
  while(size(command->args) != 0) {
    argv[i] = (char*)front(command->args);
    printf("added argument %s\n", argv[i]);
    dequeue(command->args);
    ++i;
  }
  int argc = i;
  if (checkBuiltInCommand(cmd_name, argc, argv) == 0) {
	  return 0; // Should It Return 0 ?
  }
  sprintf(PATH, "%s", getenv("PATH"));
  process = fork();
  if (process < 0) {
    fprintf(stderr, "FORK FAILED!!\n");
    free(argv);
    return ERR_FORK;
  }
  if (process == 0) { // In child
    char* path_dir = strtok(PATH, ":");
    while (path_dir) {
      if (temp) {
        free(temp);
        temp = NULL;
      }
      temp = (char*)malloc(strlen(cmd_name) + strlen(path_dir) + 2);
      strcpy(temp, path_dir);
      strcat(temp, "/");
      strcat(temp, cmd_name);
      //printf("Executing %s\n", temp);
      execvp(temp, argv); // If this returns the exec failed
      path_dir = strtok(NULL, ":");
    }
    if (path_dir == NULL) {
      if (temp) {
        free(temp);
        temp = NULL;
      }
      return ERR_NOT_FOUND;
    }
  } else if (process == 1) { // Parent
    wait(&status); // Wait for child to finish
    if (temp) {
      free(temp);
      temp = NULL;
    }
    return status;
  }
}


