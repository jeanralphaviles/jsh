%{ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int linenum = 1;

void yyerror(const char *str) {
        fprintf(stderr,"line: %d error: %s\n", linenum, str);
}

int yywrap() {
        return 1;
}



%}

%union {
  char* string;
}

/* -------------------------------------------------------
   The grammar symbols ------------------------------------------------------- */
%token<string>  WORD
%token<string>  ASSIGNMENT_WORD
%token<string>  NAME
%token<string>  NEWLINE
%token<string>  IO_NUMBER


/* The following are the operators mentioned above. */
%token<string>  AND_IF    OR_IF    DSEMI
/*              '&&'      '||'     ';;'    */
%token<string>  DLESS  DGREAT  LESSAND  GREATAND  LESSGREAT  DLESSDASH
/*              '<<'   '>>'    '<&'     '>&'      '<>'       '<<-'   */
%token<string>  CLOBBER
/*              '>|'   */
/* The following are the reserved words. */
%token<string>  If    Then    Else    Elif    Fi    Do    Done
/*              'if'  'then'  'else'  'elif'  'fi'  'do'  'done'   */
%token<string>  Case    Esac    While    Until    For
/*              'case'  'esac'  'while'  'until'  'for'   */
/* These are reserved words, not operator tokens, and are
   recognized when reserved words are recognized. */
%token<string>  Lbrace    Rbrace    Bang
/*              '{'       '}'       '!'   */
%token<string>  In
/*              'in'   */

%type<string> complete_command pipe_sequence sequence_separator single_command
%type<string> cmd_name cmd_suffix io_redirect io_file filename

/* -------------------------------------------------------
   The Grammar
   ------------------------------------------------------- */
%start  complete_command
%%
complete_command   : pipe_sequence 
                   | complete_command sequence_separator pipe_sequence
                   | complete_command '&'
                   | complete_command NEWLINE
                   ;
pipe_sequence      : single_command { printf("pipe_sequence\n"); }
                   | pipe_sequence '|' single_command
                   ;
sequence_separator : AND_IF
                   | OR_IF
                   ;
single_command     : cmd_name
                   | cmd_name cmd_suffix 
                   ;
cmd_name           : WORD {printf("Hey!\n"); }
                   ;
cmd_suffix         :            io_redirect
                   | cmd_suffix io_redirect
                   |          WORD
                   | cmd_name WORD
                   ;
io_redirect        :           io_file
                   | IO_NUMBER filename
                   ;
io_file            : '<'      filename
                   | '>'      filename
                   | GREATAND filename
                   | DGREAT   filename
                   ;
filename           : WORD
                   ;
%%
