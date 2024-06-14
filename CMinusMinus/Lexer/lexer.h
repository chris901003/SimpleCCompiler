#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <map>
#include "../Token/token.h"

class Lexer {
    private:
        int startRow;
        char currentChar;
        void getNextChar();
        Token getNextToken();
        std::map<std::string, Token> keywords;

    public:
        Lexer();
        std::vector<Token> tokens;
        int lines, rows;
        void startLexer();
};

#endif