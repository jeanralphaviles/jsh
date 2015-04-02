%{
#include <stdlib.h>

#include "ast.h"
#include "env.h"
#include "y.tab.h"
extern int linenum;
%}

%%
EOF                               { exit(EXIT_SUCCESS); }
\n+                               { ++linenum; return NEWLINE; }
"&&"                              { return AND_IF; }
"||"                              { return OR_IF; }
[|\(\)&;<>]                       { return yytext[0]; }
[0-9]+[<|>]                       { return IO_NUMBER; }
\".*\"                            { yylval.string = strdup(yytext); yylval.string[strlen(yylval.string) - 1] = '\0';
                                    yylval.string[0] = '\0'; yylval.string++; return WORD; }
[a-zA-Z0-9\-\*\.\/(\$\{.*\})]+    { yylval.string = expandVariables(strdup(yytext)); return WORD; }
[ \t\r]+                          { ; } /* Ignore Whitespace */
.                                 { ; } /* No Match */
%%
