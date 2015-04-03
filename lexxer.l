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
[0-9]>                            { strcpy(yylval.string, yytext); return IO_NUMBER; }
[|\(\)&;<>]                       { return yytext[0]; }
\".*\"                            { yylval.string = strdup(yytext+1); yylval.string[strlen(yylval.string) - 1] = '\0';
                                    return WORD; }
[a-zA-Z0-9\-\*\.\/(\$\{.*\}))_]+  { yylval.string = expandVariables(strdup(yytext)); return WORD; }
[ \t\r]+                          { ; } /* Ignore Whitespace */
.                                 { ; } /* No Match */
%%
