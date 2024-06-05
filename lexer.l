%{
#include "parser.tab.h"

int line = 1;
%}

digit [0-9]
letter [a-zA-Z]
identifier {letter}({letter}|{digit}|_)*

%%
"//".*               { /* Skip single-line comment */ }
"/*"([^*]|\*+[^*/])*\*+"/"    { /* Skip multi-line comment */ }

"main"                  { return MAIN; }
"int"                   { return INT; }
"float"                 { return FLOAT; }
"if"                    { return IF; }
"else"                  { return ELSE; }
"while"                 { return WHILE; }

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
","                     { return ','; }

{digit}+\.{digit}+      { yylval.fval = atof(yytext); return FLOATINGNUMBER; }
{digit}+                { yylval.ival = atoi(yytext); return NUMBER; }
{identifier}            {
    yylval.sval = strdup(yytext);
    return IDENTIFIER;
}

"\n"                    { line += 1; }
[ \t]+                  { /* skip whitespace */ }
.                       { printf("Unkown token\n"); }

%%
int yywrap(void) {
    return 1;
}

