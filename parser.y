%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./data_struct/identifier_data.h"
#include "./data_struct/data_type.h"

extern int yylex(void);
void yyerror(const char *s);
extern int line;
extern struct IdentifierData **idDataList;
extern void init();
%}

%union {
    int ival;
    float fval;
    char* sval;
    struct IdentifierData *idData;
    struct DataType *dataType;
}

%token <sval> INT FLOAT
%token MAIN ASSIGN PRINT
%token <idData> IDENTIFIER
%token <ival> NUMBER
%token <fval> FLOATINGNUMBER

%type <sval> type;
%type <dataType> expr; 
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
    | printStatement
    ;

declaration:
    type IDENTIFIER ASSIGN expr ';' {
        struct IdentifierData *idData = findIdentifer(idDataList, $2->name);
        if (strcmp($1, "int") == 0) {
            idData->type = 0;
            idData->iValue = $4->ival;
        } else if (strcmp($1, "float") == 0) {
            idData->type = 1;
            idData->fValue = $4->fval;
        }
    }
    ;

assignment:
    IDENTIFIER ASSIGN expr ';' {
        struct IdentifierData *idData = findIdentifer(idDataList, $1->name);
        if ($3->type == 0) {
            idData->iValue = $3->ival;
        } else if ($3->type == 1) {
            idData->fValue = $3->fval;
        } else if ($3->type == 2) {
            struct IdentifierData *target = findIdentifer(idDataList, $3->idName);
            if (target == NULL) {
                printf("Identifier: %s not declare.\n", $3->idName);
            } else {
                idData->iValue = target->iValue;
                idData->fValue = target->fValue;
            }
        }
    }
    ;

printStatement:
    PRINT '(' IDENTIFIER ')' ';' {
        struct IdentifierData *idData = findIdentifer(idDataList, $3->name);
        if (idData == NULL) {
            printf("Identifier: %s not found.\n", $3->name);
        } else {
            printf("Identifier: %s.\nInt val: %d.\nFloat val: %f\n", idData->name, idData->iValue, idData->fValue);
        }
    }
    ;

type:
    INT { $$ = "int"; }
    | FLOAT { $$ = "float"; }
    ;

expr:
    NUMBER {
        struct DataType *dataType = malloc(sizeof(struct DataType));
        dataType->type = 0;
        dataType->ival = $1;
        $$ = dataType;
    }
    | FLOATINGNUMBER {
        struct DataType *dataType = malloc(sizeof(struct DataType));
        dataType->type = 1;
        dataType->fval = $1;
        $$ = dataType;
    }
    | IDENTIFIER { 
        struct DataType *dataType = malloc(sizeof(struct DataType));
        dataType->type = 2;
        dataType->idName = $1->name;
        $$ = dataType;
    }
    ;
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(void) {
    init();
    int result = yyparse();
    printf("Total lines: %d\n", line);
    return result;
}