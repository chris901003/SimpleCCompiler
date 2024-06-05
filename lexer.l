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

"void"                  { return VOID; }
"int"                   { return INT; }
"float"                 { return FLOAT; }
"if"                    { return IF; }
"else"                  { return ELSE; }
"while"                 { return WHILE; }
"for"                   { return FOR; }
"continue"              { return CONTINUE; }
"break"                 { return BREAK; }
"return"                { return RETURN; }

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
"#include"[\t ]*"<"[a-zA-Z0-9_/\.]+">" {
    yylval.sval = strdup(yytext);
    return HEADERFILE;
}
"#include"[\t ]*"\""[a-zA-Z0-9_/\.]+"\"" {
    yylval.sval = strdup(yytext);
    return HEADERFILE;
}

"\n"                    { line += 1; }
[ \t]+                  { /* skip whitespace */ }
.                       { printf("Unkown token: %s\n", yytext); }

%%
int yywrap(void) {
    return 1;
}

