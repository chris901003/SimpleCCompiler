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

{digit}+\.{digit}+      { yylval.fval = atof(yytext); return FLOATINGNUMBER; }
{digit}+                { yylval.ival = atoi(yytext); return NUMBER; }
{identifier}            {
    struct IdentifierData *idData = malloc(sizeof(idData));
    idData->name = strdup(yytext);
    yylval.idData = idData;
    return IDENTIFIER;
}

"\n"                    { line += 1; }
[ \t]+                  { /* skip whitespace */ }
.                       { /* skip any other character */ }

%%
int yywrap(void) {
    return 1;
}

