%{
#include <stdlib.h>

#include "ast.h"
#include "env.h"
#include "utils.h"
#include "y.tab.h"
extern int linenum;
%}

%%
EOF                               { exit(EXIT_SUCCESS); }
\n+                               { ++linenum; return NEWLINE; }
"&&"                              { return AND_IF; }
"||"                              { return OR_IF; }
2>&1                              { return IO_COMBINE; }
>>                                { return DRBRKT; }
[0-9]>                            { strcpy(yylval.string, yytext); return IO_NUMBER; }
\".*\"                            { yylval.string = strdup(yytext+1); yylval.string[strlen(yylval.string) - 1] = '\0';
                                    return WORD; }
[a-zA-Z0-9\-\*\.\/\$\{\}_\\\&\<\>|\"]+  { if (isMetaCharacter(yytext[0])) return yytext[0]; else yylval.string = resolveEscapeSequences(expandVariables(strdup(yytext))); return WORD; }
[ \t\r]+                          { ; } /* Ignore Whitespace */
.                                 { ; } /* No Match */
%%

// [a-zA-Z0-9\-\*\.\/(\$\{.*\})_]+  { yylval.string = resolveEscapeSequences(expandVariables(strdup(yytext))); return WORD; }
// ((a-z)|(A-Z)|(0-9)|(\-)|(\*)|(\.)|(\/)|(\$\{.*\})|(\&)|(_))+  { yylval.string = resolveEscapeSequences(expandVariables(strdup(yytext))); return WORD; }
// [|\(\)&;<>]                       { return yytext[0]; }
