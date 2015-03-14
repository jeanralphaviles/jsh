%{
#include <stdlib.h>
#include "ast.h"
#include "y.tab.h"
extern int linenum;
%}

%%
(bye)|EOF               { exit(EXIT_SUCCESS); }
\n+                     { ++linenum; return NEWLINE; }
"&&"                    { return AND_IF; }
"||"                    { return OR_IF; }
[|\(\)&;<>]             { return yytext[0]; }
[0-9]+[<|>]             { return IO_NUMBER; } 
[a-zA-Z0-9\-\*\.\/]+      { yylval.string = strdup(yytext); return WORD; }
[ \t\r\s]+           		{ ; } /* Ignore Whitespace */
.                       { ; } /* No Match */
%%
