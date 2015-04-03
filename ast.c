#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "aliastable.h"
#include "ast.h"
#include "builtins.h"
#include "defines.h"
#include "queue.h"
#include "utils.h"

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
  ast_pipe_sequence->io_in = NULL;
  ast_pipe_sequence->io_out = NULL;
  ast_pipe_sequence->io_err = NULL;
  ast_pipe_sequence->err2out = FALSE;
  ast_pipe_sequence->append_out = FALSE;
  return ast_pipe_sequence;
}

struct AstSingleCommand* createAstSingleCommand(char* cmd_name) {
  fprintf(stderr, "Command Name coming in: %s\n", cmd_name);
  struct AstSingleCommand* ast_single_command = (struct AstSingleCommand*)malloc(sizeof(struct AstSingleCommand));

  ast_single_command->args = createQueue();
  if(checkAliasExists(cmd_name)) {
    char* alias = strdup(getAlias(cmd_name));
    char* token = strtok(alias, " ");
    int i = 0;
    while(token != NULL) {
      if (i == 0) {
        cmd_name = token;
        enqueue(ast_single_command->args, cmd_name);
      } else {
        enqueue(ast_single_command->args, token);
      }
      token = strtok(NULL, " ");
      ++i;
    }
  } else {
    enqueue(ast_single_command->args, cmd_name);
  }
  ast_single_command->cmd_name = (char*)malloc(strlen(cmd_name) + 1);
  strcpy(ast_single_command->cmd_name, cmd_name);
  return ast_single_command;
}

// Member Functions
// --AstSingleCommand--
void addArgs(struct AstSingleCommand* ast, char* arg) {
  if (ast->args == NULL) {
    ast->args = createQueue();
  }
  char* temp = malloc(strlen(arg) + 1);
  strcpy(temp, arg);
  enqueue(ast->args, temp);
}

// --AstPipeSequence--
void addCommand(struct AstPipeSequence* pipe_sequence, struct AstSingleCommand* command) {
  if (pipe_sequence->commands == NULL) {
    pipe_sequence->commands = createQueue();
  }
  enqueue(pipe_sequence->commands, command);
}

void setIoIn(struct AstPipeSequence* pipe_sequence, char* in) {
  pipe_sequence->io_in = (char*)malloc(strlen(in) + 1);
  strcpy(pipe_sequence->io_in, in);
}

void setIoOut(struct AstPipeSequence* pipe_sequence, char* out, bool append_out) {
  pipe_sequence->io_out = (char*)malloc(strlen(out) + 1);
  strcpy(pipe_sequence->io_out, out);
  pipe_sequence->append_out = append_out;
}

void setIoErr(struct AstPipeSequence* pipe_sequence, char* out) {
  if (out == NULL) {
    pipe_sequence->io_err = NULL;
    pipe_sequence->err2out = TRUE;
  } else {
    pipe_sequence->io_err = (char*)malloc(strlen(out) + 1);
    strcpy(pipe_sequence->io_err, out);
    pipe_sequence->err2out = FALSE;
  }
}

// --AstRoot--
void addPipeSequence(struct AstRoot* root, struct AstPipeSequence* pipe_sequence) {
  if (root->pipe_sequences == NULL) {
    root->pipe_sequences = createQueue();
  }
  enqueue(root->pipe_sequences, pipe_sequence);
}

void addPipeSequenceWithSeparator(struct AstRoot* root, struct AstPipeSequence* pipe_sequence, int separator) {
  if (root->sequence_separators == NULL) {
    root->sequence_separators = createQueue();
  }
  if (root->pipe_sequences == NULL || size(root->pipe_sequences) == 0 || size(root->pipe_sequences) != size(root->sequence_separators) + 1) {
    fprintf(stderr, "PipeSequences and Separators are messed up\n");
  }
  enqueue(root->pipe_sequences, pipe_sequence);
  enqueue(root->sequence_separators, (void*)(unsigned long)separator); // LOL, portability
}

// Execute
int executeAstRoot(struct AstRoot* root) {
  if (root == NULL || size(root->pipe_sequences) == 0) {
    return ERR_NOT_FOUND;
  } else {
    bool async = root->async;
    if (async) {
      if (fork() > 0) { // Fork and let the children work
        return SUCCESS;
      }
    }
    int status;
    int sequence_separator;
    struct AstPipeSequence* pipe_sequence;
    struct queue* pipe_sequences = root->pipe_sequences;
    struct queue* sequence_separators = root->sequence_separators;
    while (size(pipe_sequences) > 0) {
      pipe_sequence = front(pipe_sequences);
      dequeue(pipe_sequences);
      status = executePipeSequence(pipe_sequence);
      if (size(sequence_separators) > 0) {
        sequence_separator = (int)(unsigned long)front(sequence_separators); // Also LOL
        dequeue(sequence_separators);
        if ((sequence_separator == DAND && status != SUCCESS) ||
            (sequence_separator == DPIPE && status == SUCCESS)) {
          if (async == TRUE) { // Don't want to return if we're the child
            exit(EXIT_FAILURE);
          }
          return ERR_SEQUENCE;
        }
      }
    }
    if (async == TRUE) { // Don't want to return if we're the child
      exit(EXIT_SUCCESS);
    }
    return status;
  }
}

int executePipeSequence(struct AstPipeSequence* pipe_sequence) {
  if (size(pipe_sequence->commands) == 0) {
    return ERR_NOT_FOUND;
  } else {
    struct queue* commands = pipe_sequence->commands;
    int currPipe[2] = {-1, -1};
    int lastPipe = -1;
    int i;
    pid_t* pids = (pid_t*)malloc(sizeof(pid_t)*size(commands));

    // File IO
    int old_stdin = dup(STDIN_FILENO);
    int old_stdout = dup(STDOUT_FILENO);
    int old_stderr = dup(STDERR_FILENO);
    char* io_in = pipe_sequence->io_in;
    if (io_in != NULL) {
      if (freopen(io_in, "r", stdin) == NULL) {
        char* oldColor = setTermColor(KRED);
        fprintf(stderr, "Cannot open %s as File IO_IN\n", io_in);
        setTermColor(oldColor);
        return ERR_PERMISSION;
      }
    }
    char* io_out = pipe_sequence->io_out;
    if (io_out != NULL) {
      char* file_mode = pipe_sequence->append_out ? "a" : "w";
      if (freopen(io_out, file_mode, stdout) == NULL) {
        char* oldColor = setTermColor(KRED);
        fprintf(stderr, "Cannot open %s as File IO_OUT\n", io_out);
        setTermColor(oldColor);
        return ERR_PERMISSION;
      }
    }
    char* io_err = pipe_sequence->io_err;
    if (io_err != NULL) {
      int fd;
      if ((fd = open(io_err, O_CREAT|O_TRUNC|O_WRONLY, 0777)) == -1) {
        char* oldColor = setTermColor(KRED);
        fprintf(stderr, "Cannot open %s as File IO_ERR\n", io_err);
        setTermColor(oldColor);
        return ERR_PERMISSION;
      } else {
        dup2(fd, STDERR_FILENO);
      }
    } else if (pipe_sequence->err2out) {
      dup2(STDOUT_FILENO, STDERR_FILENO);
    }

    for (i = 0; size(commands) > 0; ++i) {
      // Get next command
      struct AstSingleCommand* command = (struct AstSingleCommand*)front(commands);
      dequeue(commands);
      char* cmd_name = command->cmd_name;
      if (size(commands) > 0) { // Don't need to pipe if we're the last command
        pipe(currPipe);
      }
      if (isBuiltinCommand(cmd_name)) {
        int argc = size(command->args);
        char** argv = getArgs(command);
        executeBuiltinCommand(cmd_name, argc, argv);
        int j = 0;
        while (argv[j] != NULL) {
           free(argv[j++]);
        }
        free(argv);
        free(cmd_name);
        continue;
      }
      switch (pids[i] = fork()) {
        case -1:
          fprintf(stderr, "failed to fork\n");
          break;
        case 0: // In child
          // Re-enable Interrupts
          signal(SIGINT, SIG_DFL);  /* enable ctrl+C */
          signal(SIGQUIT, SIG_DFL); /* enable ctrl-\ */
          if (lastPipe >= 0) {
            // We have a pipe to read from
            dup2(lastPipe, STDIN_FILENO); // stdin <= lastpipe
            close(lastPipe);
          }
          if (currPipe[WRITE_END] >= 0) {
            // If we have a pipe to write to use it
            dup2(currPipe[WRITE_END], STDOUT_FILENO);
            // Close both ends of the pipe here
            close(currPipe[WRITE_END]);
            close(currPipe[READ_END]);
          }
          executeCommand(command); // Will not return unless there is an error
          exit(EXIT_FAILURE);
          break;
        default: // In parent
          // Child has handled the write end
          if (currPipe[WRITE_END] >= 0) {
            close(currPipe[WRITE_END]);
          }
          if (lastPipe >= 0) {
            close(lastPipe);
          }
          lastPipe = currPipe[READ_END];
          // Zero out pipe
          currPipe[READ_END] = -1;
          currPipe[WRITE_END] = -1;
          break;
      }
    }
    // Wait for every process to finish. Record if any failed.
    bool failed = FALSE;
    while (i--) {
      int status;
      if (wait(&status) == -1 || status != SUCCESS){
        failed = TRUE;
      }
    }
    free(pids);
    // Close Files used for IO and reopen streams
    if (io_in != NULL) {
      fflush(stdin);
      dup2(old_stdin, STDIN_FILENO);
      free(io_in);
    }
    if (io_out != NULL) {
      fflush(stdout);
      dup2(old_stdout, STDOUT_FILENO);
      free(io_out);
    }
    if (io_err != NULL || pipe_sequence->err2out) {
      fflush(stderr);
      dup2(old_stderr, STDERR_FILENO);
      free(io_err);
    }
    return failed == TRUE ? ERROR : SUCCESS;
  }
}

void executeCommand(struct AstSingleCommand* command) {
  char* cmd_name = command->cmd_name;
  char** argv = getArgs(command);
  // i.e. if ths command was /bin/ls
  if (isAbsolutePath(cmd_name)) {
    if (fileExists(cmd_name)) {
        execv(cmd_name, argv); // Will not return, unless it fails
    }
    int i = 0;
    while (argv[i] != NULL) {
      free(argv[i++]);
    }
    free(argv);
    free(cmd_name);
    exit(EXIT_FAILURE);
  }
  char* path = getenv("PATH");
  char* token = strtok(path, ":");
  while (token != NULL) {
    // Allocate enough space for strcat + NULL + '/'
    char* filename = (char*)malloc(strlen(token) + strlen(cmd_name) + 2);
    strcpy(filename, token);
    strcat(filename, "/");
    strcat(filename, cmd_name);
    if (fileExists(filename)) {
      execv(filename, argv); // Will not return, unless it fails
      perror("wtf");
      exit(EXIT_FAILURE);
    } else {
      free(filename);
      token = strtok(NULL, ":");
    }
  }
  const char* oldTermColor = setTermColor(stderr, KRED);
  fprintf(stderr, "jsh: command not found: %s\n", cmd_name);
  setTermColor(stderr, oldTermColor);
  free(argv);
  free(cmd_name);
  exit(EXIT_FAILURE);
}

bool fileExists(char* filename) {
  struct stat buff;
  return stat(filename, &buff) == 0 && buff.st_mode & S_IXUSR;
}

char** getArgs(struct AstSingleCommand* command) {
  int i;
  char** argv = (char**) malloc(sizeof(char**) * (size(command->args) + 1));
  for (i = 0; size(command->args) > 0; ++i) {
    argv[i] = (char*)front(command->args);
    dequeue(command->args);
  }
  argv[i] = NULL;
  return argv;
}
