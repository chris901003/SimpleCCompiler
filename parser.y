%{
#include <stdio.h>
#include <stdlib.h>

void yyerror(const char *s);
int yylex();
%}

%union {
    int intval;
}

%token <intval> NUMBER
%token PLUS END

%type <intval> expr

%%

input:
    expr END { printf("Result: %d\n", $1); }
    ;

expr:
    expr PLUS expr { $$ = $1 + $3; }
    | NUMBER       { $$ = $1; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    printf("Enter expressions to add (e.g., 2 + 3):\n");
    return yyparse();
}