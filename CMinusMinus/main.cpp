#include <iostream>
#include "./Lexer/lexer.h"

using namespace std;

int main() {
    cout << "Starting C-- Compiler" << endl;
    Lexer *lexer = new Lexer();
    lexer->startLexer();
    cout << "Tokens: " << lexer->tokens.size() << endl;
    return 0;
}