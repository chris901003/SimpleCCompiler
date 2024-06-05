%{
#include <stdio.h>
#include <stdlib.h>
#define YYERROR_VERBOSE 1

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
%token WHILE FOR
%token BREAK CONTINUE

%%

Program:
    INT MAIN '(' ')' Block
    ;

Block:
    '{' Statements '}'
    ;

Statements:
        /* empty */
    |Statements Statement
    ;

Statement:
    DeclarationStatement
    | AssignmentStatement
    ;

DeclarationStatement:
    VariableType DeclarationVariableList ';'
    ;

VariableType:
    INT
    | FLOAT
    ;

DeclarationVariableList:
    DeclarationVariable
    | DeclarationVariableList ',' DeclarationVariable
    ;

DeclarationVariable:
    IDENTIFIER
    | IDENTIFIER ASSIGN expression
    ;

AssignmentExpression:
    IDENTIFIER ASSIGN expression
    ;

AssignmentStatement:
    AssignmentExpression ';'
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
    NUMBER
    | FLOATINGNUMBER
    | IDENTIFIER
    | '(' expression ')'
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