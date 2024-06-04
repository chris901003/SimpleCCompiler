%{
#include "parser.tab.h"

int line = 1;
%}

digit [0-9]
letter [a-zA-Z]
identifier {letter}({letter}|{digit}|_)*

%%

"main"                  { return MAIN; }
"int"                   { return INT; }
"float"                 { return FLOAT; }

"="                     { return ASSIGN; }
"{"                     { return '{'; }
"}"                     { return '}'; }
"("                     { return '('; }
")"                     { return ')'; }
";"                     { return ';'; }

{digit}+\.{digit}+      { yylval.sval = strdup(yytext); return FLOATINGNUMBER; }
{digit}+                { yylval.sval = strdup(yytext); return NUMBER; }
{identifier}            { yylval.sval = strdup(yytext); return IDENTIFIER; }

"\n"                    { line += 1; }
[ \t]+                  { /* skip whitespace */ }
.                       { /* skip any other character */ }

%%
int yywrap(void) {
    return 1;
}

