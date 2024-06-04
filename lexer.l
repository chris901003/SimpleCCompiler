%{
#include "parser.tab.h"

int line = 1;
%}

digit   [0-9]
letter  [a-zA-Z]
identifier {letter}({letter}|{digit}|_)*

%%

"int"                   { return INT; }
"main"                  { return MAIN; }


"{"                     { return '{'; }
"}"                     { return '}'; }
"("                     { return '('; }
")"                     { return ')'; }
";"                     { return ';'; }


{digit}+                { return NUMBER; }
{identifier}            { yylval.sval = strdup(yytext); return IDENTIFIER; }


"\n"                    { line += 1; }
[ \t]+                  { /* skip whitespace */ }
.                       { /* skip any other character */ }

%%
int yywrap(void) {
    return 1;
}

