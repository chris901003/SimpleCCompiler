#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <utility>

enum TokenType {
    Identifier,
    IntValue,
    Void, Int,
    IF, ELSE, WHILE, FOR,
    Plus, Minus, Multiply, Divide, Modulus,
    LessThan, LessThanEqual, GreaterThan, GreaterThanEqual, Equal, NotEqual,
    ASSIGN, LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, SEMICOLON, COMMA,
    RETURN, BREAK, CONTINUE,
    EOF_TOKEN, UNKNOWN
};

class Token {
    public:
        TokenType type;
        std::string sValue;
        int iValue;

        Token(TokenType type = UNKNOWN, std::string sValue = "", int iValue = 0) {
            this->type = type;
            this->sValue = sValue;
            this->iValue = iValue;
        }

        std::string toString() {
            std::string result = "Token(" + std::to_string(this->type);
            if (this->type == IntValue) {
                result += ", " + std::to_string(this->iValue);
            } else {
                result += ", " + this->sValue;
            }
            result += ")";
            return result;
        }
};

#endif