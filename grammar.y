%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "defines.h"

int linenum = 1;
struct AstRoot* astRoot; // Contains parsed command

void yyerror(const char *str) {
        fprintf(stderr,"line: %d error: %s\n", linenum, str);
}

int yywrap() {
        return 1;
}
%}

%union {
  int integer;
  char* string;
  struct AstRoot* astRoot;
  struct AstPipeSequence* astPipeSequence;
  struct AstSingleCommand* astSingleCommand;
}

/* -------------------------------------------------------
   The grammar symbols ------------------------------------------------------- */
%token<string>  WORD
%token<string>  ASSIGNMENT_WORD
%token<string>  NAME
%token<string>  NEWLINE
%token<string>  IO_NUMBER


%token<integer>  AND_IF    OR_IF
/*              '&&'      '||' */

%type<integer> sequence_separator
%type<string>  filename cmd_name io_in io_out
%type<astSingleCommand> single_command
%type<astPipeSequence> pipe_sequence
%type<astRoot> complete_command
/* -------------------------------------------------------
   The Grammar
   ------------------------------------------------------- */
%start  complete_command
%%
complete_command   : pipe_sequence {$$ = createAstRoot(); addPipeSequence($$, $1); astRoot = $$;}
                   | complete_command sequence_separator pipe_sequence {addPipeSequenceWithSeparator($1, $3, ($2 == AND_IF) ? 1 : 2); $$ = $1; astRoot = $$;}
                   | complete_command '&' {$$->async = TRUE; astRoot = $$;}
                   | complete_command NEWLINE {$$ = $1; astRoot = $$;}
                   | NEWLINE {$$ = createAstRoot(); astRoot = $$;}
                   ;
pipe_sequence      : single_command {$$ = createAstPipeSequence(); addCommand($$, $1);}
                   | pipe_sequence '|' single_command {addCommand($1, $3); $$ = $1;}
                   ;
sequence_separator : AND_IF {$$ = $1;}
                   | OR_IF {$$ = $1;}
                   ;
single_command     : cmd_name {$$ = createAstSingleCommand($1, NULL, NULL);}
                   | single_command io_in {setIoIn($1, $2); $$ = $1;}
                   | single_command io_out {setIoOut($1, $2); $$ = $1;}
                   | single_command WORD {addArgs($1, $2); $$ = $1;}
                   ;
cmd_name           : WORD {$$ = $1;}
                   ;
io_in              : '<'      filename {$$ = $2;}
                   ;
io_out             : '>'      filename {$$ = $2;}
                   ;
filename           : WORD {$$ = $1;}
                   ;
%%
