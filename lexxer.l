%{
#include <stdlib.h>

#include "ast.h"
#include "defines.h"
#include "env.h"
#include "utils.h"
#include "y.tab.h"

extern int linenum;
extern bool inString;
%}

%%
EOF                               { exit(EXIT_SUCCESS); }
\n+                               { ++linenum; return NEWLINE; }
"&&"                              { return AND_IF; }
"||"                              { return OR_IF; }
2>&1                              { return IO_COMBINE; }
>>                                { return DRBRKT; }
[0-9]>                            { strcpy(yylval.string, yytext); return IO_NUMBER; }
[|\(\)&;<>]                       { return yytext[0];  }
\"(\\.|[^"])*\"                   { yylval.string = strdup(yytext+1); yylval.string[strlen(yylval.string) - 1] = '\0'; inString = TRUE;
                                    return WORD; }
\'(\\.|[^'])*\'                   { yylval.string = strdup(yytext+1); yylval.string[strlen(yylval.string) - 1] = '\0'; inString = TRUE;
                                    return WORD; }
([a-zA-Z0-9\-\*\.\/\$\{\}_~?:]*|(\\[\\\&|\<\>\"\' a-zA-Z0-9\$\{\}])*)+  { yylval.string = resolveEscapeSequences(expandVariables(strdup(yytext))); return WORD; }
[ \t\r]+                          { ; } /* Ignore Whitespace */
.                                 { ; } /* No Match */
%%
