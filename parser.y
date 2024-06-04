%{
#include <stdio.h>
#include <stdlib.h>

extern int yylex(void);
void yyerror(const char *s);
extern int line;

IdentifierData *idDataList[1000];
%}

%union {
    int ival;
    float fval;
    char* sval;
    struct IdentifierData {
        char *name;
        int hash;

        // 0: int, 1: float
        int type;
        int iValue;
        float fValue;
        IdentifierData *nxt;
    } *idData;
}

%token <sval> INT FLOAT
%token MAIN ASSIGN
%token <idData> IDENTIFIER
%token <ival> NUMBER
%token <fval> FLOATINGNUMBER
%%

program:
    INT MAIN '(' ')' '{' statements '}'
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
    type IDENTIFIER ASSIGN expr ';'
    ;

assignment:
    IDENTIFIER ASSIGN expr ';'
    ;

type:
    INT
    | FLOAT
    ;

expr:
    NUMBER
    | FLOATINGNUMBER
    | IDENTIFIER
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