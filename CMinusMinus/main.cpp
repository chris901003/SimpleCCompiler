#include <iostream>
#include "./Lexer/lexer.h"
#include "./Parser/parser.h"

using namespace std;

int main() {
    cout << "Starting C-- Compiler" << endl;
    Lexer *lexer = new Lexer();
    lexer->startLexer();
    Parser *parser = new Parser(lexer->tokens);
    parser->startParse();
    return 0;
}