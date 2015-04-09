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
#define KNRM  "\001\033[0m\002"
#define KRED  "\001\033[31m\002"
#define KGRN  "\001\033[32m\002"
#define KYEL  "\001\033[33m\002"
#define KBLU  "\001\033[34m\002"
#define KMAG  "\001\033[35m\002"
#define KCYN  "\001\033[36m\002"
#define KWHT  "\001\033[37m\002"

#endif
