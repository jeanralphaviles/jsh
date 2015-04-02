#ifndef DEFINES_H
#define DEFINES_H

#define bool short
#define TRUE 1
#define FALSE 0
#define MAX_LENGTH 1024

// Sequence Separators
#define DAND 0 // &&
#define DPIPE 1 // ||

// File Descriptor
#define READ_END 0
#define WRITE_END 1

// Stdio
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

// Errors
#define ERR_NOT_FOUND 127
#define ERR_FORK 126
#define ERR_SEQUENCE 125
#define ERR_PERMISSION 124
#define ERROR 123
#define SUCCESS 0

// Regex
#define ENVIRONMENTAL_VARIABLE_REGEX "\\$\\{.*\\}"

// Colors
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#endif
