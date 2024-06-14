#include <vector>
#include <iostream>
#include "./parser.h"

Parser::Parser(std::vector<Token> tokens) {
    this->tokens = tokens;
}

void Parser::startParse() {
    for (Token token : this->tokens) {
        std::cout << "Token: " << token.toString() << std::endl;
    }
}