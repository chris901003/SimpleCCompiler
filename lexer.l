%{
#include "parser.tab.h"
#include "./data_struct/identifier_data.h"

int line = 1;
struct IdentifierData **idDataList;
%}

digit [0-9]
letter [a-zA-Z]
identifier {letter}({letter}|{digit}|_)*

%%

"main"                  { return MAIN; }
"int"                   { return INT; }
"float"                 { return FLOAT; }
"print"                 { return PRINT; }

"="                     { return ASSIGN; }
"{"                     { return '{'; }
"}"                     { return '}'; }
"("                     { return '('; }
")"                     { return ')'; }
";"                     { return ';'; }

{digit}+\.{digit}+      { yylval.fval = atof(yytext); return FLOATINGNUMBER; }
{digit}+                { yylval.ival = atoi(yytext); return NUMBER; }
{identifier}            {
    char *name = strdup(yytext);
    struct IdentifierData *idData = findIdentifer(idDataList, name);
    if (idData == NULL) {
        idData = malloc(sizeof(struct IdentifierData));
        idData->name = strdup(yytext);
        saveIdentifier(idDataList, idData);
    }
    yylval.idData = idData;
    return IDENTIFIER;
}

"\n"                    { line += 1; }
[ \t]+                  { /* skip whitespace */ }
.                       { /* skip any other character */ }

%%
int yywrap(void) {
    return 1;
}

void init() {
    idDataList = initIdentifierList();
}

