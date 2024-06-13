#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include "../Token/token.h"

class Lexer {
    private:
        char currentChar;
        void getNextChar();
        Token getNextToken();

    public:
        Lexer();
        std::vector<Token> tokens;
        void startLexer();
};

#endif