#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenType {
    Identifier,
    Keyword
};

class Token {
    public:
        TokenType type;
        std::string value;
        Token(TokenType type, std::string value) {
            this->type = type;
            this->value = value;
        }

        std::string toString() {
            return "Token(" + std::to_string(this->type) + ", " + this->value + ")";
        }
};

#endif