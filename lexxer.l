%{
#include <stdlib.h>
#include "y.tab.h"
extern int linenum;
// 
%}

%%
\n+			{ return NEWLINE; }
(bye)|EOF               { exit(EXIT_SUCCESS); }
"&&"                    { return AND_IF; }
"||"                    { return OR_IF; }
";;"                    { return DSEMI; }
"<<"                    { return DLESS; }
">>"                    { return DGREAT; }
"<&"                    { return LESSAND; }
">&"                    { return GREATAND; }
"<>"                    { return LESSGREAT; }
"<<-"                   { return DLESSDASH; }
">|"                    { return CLOBBER; }
[|\(\)&;<>]             { return yytext[0]; }
if                      { return If; }
then                    { return Then; }
else                    { return Else; }
elif                    { return Elif; }
fi                      { return Fi; }
do                      { return Do; }
done                    { return Done; }
case                    { return Case; }
esac                    { return Esac; }
while                   { return While; }
until                   { return Until; }
for                     { return For; }
in                      { return In; }
"{"                     { return Lbrace; }
"}"                     { return Rbrace; }
"!"                     { return Bang; }
[a-zA-Z][a-zA-Z0-9]*    { yylval.string = strdup(yytext); return WORD; }
[0-9]+[<|>]             { return IO_NUMBER; } 
[ \t\r]+		{ ; } /* Ignore Whitespace */
.                       { ; } /* No Match */
%%
