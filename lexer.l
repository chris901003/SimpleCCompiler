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
"if"                    { return IF; }
"else"                  { return ELSE; }

"="                     { return ASSIGN; }
"{"                     { return '{'; }
"}"                     { return '}'; }
"("                     { return '('; }
")"                     { return ')'; }
";"                     { return ';'; }
"+"                     { return '+'; }
"-"                     { return '-'; }
"*"                     { return '*'; }
"/"                     { return '/'; }
"<"                     { return '<'; }
">"                     { return '>'; }
"=="                    { return EQ; }
">="                    { return GE; }
"<="                    { return LE; }
"!="                    { return NE; }
"%"                     { return '%'; }

{digit}+\.{digit}+      { yylval.fval = atof(yytext); return FLOATINGNUMBER; }
{digit}+                { yylval.ival = atoi(yytext); return NUMBER; }
{identifier}            {
    yylval.sval = strdup(yytext);
    return IDENTIFIER;
}

"\n"                    { line += 1; }
[ \t]+                  { /* skip whitespace */ }
.                       { printf("Unkown token"); }

%%
int yywrap(void) {
    return 1;
}

