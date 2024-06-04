%{
#include "parser.tab.h"
%}

%%

[0-9]+      { yylval.intval = atoi(yytext); return NUMBER; }
"+"         { return PLUS; }
"\n"        { return END; }
[ \t]+      { /* ©¿²¤ªÅ¥Õ¦r²Å */ }
.           { return yytext[0]; }

%%

int yywrap() {
    return 1;
}