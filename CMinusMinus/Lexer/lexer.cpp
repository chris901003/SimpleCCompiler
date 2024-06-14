#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include "lexer.h"
#include "../Token/token.h"

Lexer::Lexer() {
    this->currentChar = ' ';
    this->lines = 1;
    this->rows = 1;
    this->startRow = 0;
    this->tokens = std::vector<Token>();
    std::vector<std::pair<std::string, Token>> keywordList = {
        {"if", Token(IF, "if")},
        {"else", Token(ELSE, "else")},
        {"while", Token(WHILE, "while")},
        {"for", Token(FOR, "for")},
        {"void", Token(Void, "void")},
        {"int", Token(Int, "int")},
        {"float", Token(Float, "float")},
        {"return", Token(RETURN, "return")},
        {"break", Token(BREAK, "break")},
        {"continue", Token(CONTINUE, "continue")}
    };
    for (auto keyword : keywordList) {
        this->keywords[keyword.first] = keyword.second;
    }
}

void Lexer::getNextChar() {
    this->currentChar = getchar();
    if (this->currentChar == '\n') {
        this->rows = 0;
        this->lines++;
    } else if (this->currentChar == '\t') {
        this->rows += 4;
    } else {
        this->rows++;
    }
}

Token Lexer::getNextToken() {
    while (this->currentChar == ' ' || this->currentChar == '\n' || this->currentChar == '\t') {
        this->getNextChar();
    }
    if (this->currentChar == EOF) {
        return Token(EOF_TOKEN, "EOF");
    }
    this->startRow = this->rows - 1;

    // Check for identifiers
    if (isalpha(this->currentChar) || this->currentChar == '_') {
        std::string identifier = "";
        while (isalnum(this->currentChar)) {
            identifier += this->currentChar;
            this->getNextChar();
        }
        if (this->keywords.count(identifier)) {
            return this->keywords[identifier];
        }
        Token token(Identifier);
        token.sValue = identifier;
        return token;
    }

    // Check for integers and floats
    if (isdigit(this->currentChar)) {
        std::string number = "";
        while (isdigit(this->currentChar)) {
            number += this->currentChar;
            this->getNextChar();
        }
        if (this->currentChar == '.') {
            number += this->currentChar;
            this->getNextChar();
            while (isdigit(this->currentChar)) {
                number += this->currentChar;
                this->getNextChar();
            }
            Token token(FloatValue);
            token.fValue = std::stof(number);
            return token;
        }
        Token token(IntValue);
        token.iValue = std::stoi(number);
        return token;
    }

    // Check for operators
    switch (this->currentChar) {
        case '+':
            this->getNextChar();
            return Token(Plus, "+");
        case '-':
            this->getNextChar();
            return Token(Minus, "-");
        case '*':
            this->getNextChar();
            return Token(Multiply, "*");
        case '/':
            this->getNextChar();
            return Token(Divide, "/");
        case '%':
            this->getNextChar();
            return Token(Modulus, "%");
        case '<':
            this->getNextChar();
            if (this->currentChar == '=') {
                this->getNextChar();
                return Token(LessThanEqual, "<=");
            }
            return Token(LessThan, "<");
        case '>':
            this->getNextChar();
            if (this->currentChar == '=') {
                this->getNextChar();
                return Token(GreaterThanEqual, ">=");
            }
            return Token(GreaterThan, ">");
        case '=':
            this->getNextChar();
            if (this->currentChar == '=') {
                this->getNextChar();
                return Token(Equal, "==");
            }
            return Token(ASSIGN, "=");
        case '!':
            this->getNextChar();
            if (this->currentChar == '=') {
                this->getNextChar();
                return Token(NotEqual, "!=");
            }
            return Token(UNKNOWN);
        case '(':
            this->getNextChar();
            return Token(LEFT_PAREN, "(");
        case ')':
            this->getNextChar();
            return Token(RIGHT_PAREN, ")");
        case '{':
            this->getNextChar();
            return Token(LEFT_BRACE, "{");
        case '}':
            this->getNextChar();
            return Token(RIGHT_BRACE, "}");
        case ';':
            this->getNextChar();
            return Token(SEMICOLON, ";");
        case ',':
            this->getNextChar();
            return Token(COMMA, ",");
    }

    return Token(UNKNOWN);
}

void Lexer::startLexer() {
    this->getNextChar();
    while (true) {
        this->tokens.push_back(this->getNextToken());
        if (this->tokens.back().type == UNKNOWN) {
            std::cout << "Unknown token at line " << this->lines << " and row " << this->startRow << std::endl;
            return;
        }
        std::cout << "Token: " << this->tokens.back().toString() << " at line " << this->lines << " and row " << this->startRow << std::endl;
        if (this->tokens.back().type == EOF_TOKEN) {
            break;
        }
    }
    std::cout << "Tokens: " << this->tokens.size() << std::endl;
    std::cout << "Total lines: " << this->lines << std::endl;
}