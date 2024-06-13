#include <iostream>
#include "lexer.h"

Lexer::Lexer() {
    this->currentChar = ' ';
    this->tokens = std::vector<Token>();
}

void Lexer::getNextChar() {
    this->currentChar = getchar();
}

Token Lexer::getNextToken() {
    this->getNextChar();
    return Token(Identifier, "test");
}

void Lexer::startLexer() {
    this->tokens.push_back(this->getNextToken());
}