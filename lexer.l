%{
#include "parser.tab.h"

int lines = 1;
int row = 0;
void printToken(char *);
void moveRow(int num);

char *keyword = "Keyword";
char *assign = "Assign";
char *token = "Token";
%}

digit [0-9]
letter [a-zA-Z]
identifier {letter}({letter}|{digit}|_)*

%%
"//".*               { /* Skip single-line comment */ }
"/*"([^*]|\*+[^*/])*\*+"/"    { /* Skip multi-line comment */ }

"void"                  { moveRow(4); printToken(keyword); return VOID; }
"int"                   { moveRow(3); printToken(keyword); return INT; }
"float"                 { moveRow(5); printToken(keyword); return FLOAT; }
"if"                    { moveRow(2); printToken(keyword); return IF; }
"else"                  { moveRow(4); printToken(keyword); return ELSE; }
"while"                 { moveRow(5); printToken(keyword); return WHILE; }
"for"                   { moveRow(3); printToken(keyword); return FOR; }
"continue"              { moveRow(8); printToken(keyword); return CONTINUE; }
"break"                 { moveRow(5); printToken(keyword); return BREAK; }
"return"                { moveRow(6); printToken(keyword); return RETURN; }

"="                     { moveRow(1); printToken(assign); return ASSIGN; }
"{"                     { moveRow(1); printToken(token); return '{'; }
"}"                     { moveRow(1); printToken(token); return '}'; }
"("                     { moveRow(1); printToken(token); return '('; }
")"                     { moveRow(1); printToken(token); return ')'; }
";"                     { moveRow(1); printToken(token); return ';'; }
"+"                     { moveRow(1); printToken(token); return '+'; }
"-"                     { moveRow(1); printToken(token); return '-'; }
"*"                     { moveRow(1); printToken(token); return '*'; }
"/"                     { moveRow(1); printToken(token); return '/'; }
"<"                     { moveRow(1); printToken(token); return '<'; }
">"                     { moveRow(1); printToken(token); return '>'; }
"=="                    { moveRow(2); printToken(token); return EQ; }
">="                    { moveRow(2); printToken(token); return GE; }
"<="                    { moveRow(2); printToken(token); return LE; }
"!="                    { moveRow(2); printToken(token); return NE; }
"%"                     { moveRow(1); printToken(token); return '%'; }
","                     { moveRow(1); printToken(token); return ','; }

{digit}+\.{digit}+      { 
    printToken("Float number");
    yylval.fval = atof(yytext);
    moveRow(strlen(yytext));
    return FLOATINGNUMBER; 
}
{digit}+                { 
    printToken("Int number"); 
    yylval.ival = atoi(yytext); 
    moveRow(strlen(yytext));
    return NUMBER; 
}
{identifier}            {
    printToken("Identifier");
    yylval.sval = strdup(yytext);
    moveRow(strlen(yytext));
    return IDENTIFIER;
}
"#include"[\t ]*"<"[a-zA-Z0-9_/\.]+">" {
    printToken("Include");
    yylval.sval = strdup(yytext);
    moveRow(strlen(yytext));
    return HEADERFILE;
}
"#include"[\t ]*"\""[a-zA-Z0-9_/\.]+"\"" {
    printToken("Include");
    yylval.sval = strdup(yytext);
    moveRow(strlen(yytext));
    return HEADERFILE;
}

"\n"                    { lines += 1; row = 0;}
" "                     { moveRow(1); }
"\t"                    { moveRow(4); }
.                       { printf("Unkown token: %s\n", yytext); }

%%
int yywrap(void) {
    return 1;
}

void printToken(char *type) {
    printf("%8s\t: %3d\t%s\n", type, lines, yytext);
}

void moveRow(int num) {
    row += num;
}

