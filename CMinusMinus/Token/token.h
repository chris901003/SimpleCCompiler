#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <utility>

enum TokenType {
    Identifier,
    IntValue, FloatValue,
    Void, Int, Float,
    IF, ELSE, WHILE, FOR,
    Plus, Minus, Multiply, Divide, Modulus,
    LessThan, LessThanEqual, GreaterThan, GreaterThanEqual, Equal, NotEqual,
    ASSIGN, LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, SEMICOLON, COMMA,
    RETURN,
    EOF_TOKEN, UNKNOWN
};

class Token {
    public:
        TokenType type;
        std::string sValue;
        int iValue;
        float fValue;

        Token(TokenType type = UNKNOWN, std::string sValue = "", int iValue = 0, float fValue = 0.0) {
            this->type = type;
            this->sValue = sValue;
            this->iValue = iValue;
            this->fValue = fValue;
        }

        std::string toString() {
            std::string result = "Token(" + std::to_string(this->type);
            if (this->type == IntValue) {
                result += ", " + std::to_string(this->iValue);
            } else if (this->type == FloatValue) {
                result += ", " + std::to_string(this->fValue);
            } else {
                result += ", " + this->sValue;
            }
            result += ")";
            return result;
        }
};

#endif