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
    char* sval;
}

%token INT FLOAT
%token MAIN ASSIGN
%token EQ GE LE NE
%token IF ELSE
%token IDENTIFIER
%token NUMBER
%token FLOATINGNUMBER

%%

program:
    INT MAIN '(' ')' '{' statements '}'
    ;

statements:
    statements statement
    |
    ;

statement:
    declarationList
    | assignment
    | compoundStatement
    | ifStatement
    ;

compoundStatement:
    '{' statements '}'
    ;

declarationList:
    declaration
    | declarationList declaration
    ;

declaration:
    type init_declarator_list ';'
    ;

init_declarator_list:
    init_declarator
    ;

init_declarator:
    IDENTIFIER
    | IDENTIFIER ASSIGN expression
    ;

assignment:
    IDENTIFIER ASSIGN expression ';'
    ;

type:
    INT
    | FLOAT
    ;

expression:
    term
    | expression '+' term
    | expression '-' term
    ;

term:
    factor
    | term '*' factor
    | term '/' factor
    | term '%' factor
    ;

factor:
    expr
    ;

expr:
    NUMBER
    | FLOATINGNUMBER
    | IDENTIFIER
    | '(' expression ')'
    ;

ifStatement:
    IF '(' condition ')' statement ELSE statement
    | IF '(' condition ')' statement
    ;

condition:
    expression EQ expression
    | expression GE expression
    | expression LE expression
    | expression NE expression
    | expression '<' expression
    | expression '>' expression
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