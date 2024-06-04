%{
#include <stdio.h>
#include <stdlib.h>

extern int yylex(void);
void yyerror(const char *s);
extern int line;
%}

%union {
    int ival;
    float fval;
    char *sval;
}

%token <sval> INT FLOAT
%token MAIN ASSIGN
%token <sval> IDENTIFIER
%token <sval> NUMBER
%token <sval> FLOATINGNUMBER

%type <sval> expr
%type <sval> type
%%

program:
    INT MAIN '(' ')' '{' statements '}'        { printf("Valid program\n"); }
    ;

statements:
    statements statement
    |
    ;

statement:
    declaration
    | assignment
    ;

declaration:
    type IDENTIFIER ASSIGN expr ';' { printf("Declare %s %s = %s\n", $1, $2, $4); }
    ;

assignment:
    IDENTIFIER ASSIGN expr ';' { printf("Assign %s = %s\n", $1, $3); }
    ;

type:
    INT     { $$ = "int"; }
    | FLOAT  { $$ = "float"; }
    ;

expr:
    NUMBER { $$ = $1; }
    | FLOATINGNUMBER { $$ = $1; }
    | IDENTIFIER { $$ = $1; }
    ;
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(void) {
    int result = yyparse();
    printf("Total lines: %d\n", line);
    return result;
}