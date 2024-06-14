#include <vector>
#include <iostream>
#include "./parser.h"

Parser::Parser(std::vector<Token> tokens) {
    this->tokens = tokens;
}

void Parser::getNextToken() {
    this->currentToken = this->tokens[this->tokenIndex++];
}

void Parser::GlobalStatements() {
    // Globalstatments -> Globalstatment Globalstatments | £`
    this->GlobalStatement();
    if (this->currentToken.type != EOF_TOKEN) {
        this->GlobalStatements();
    }
}

void Parser::GlobalStatement() {
    // Globalstatment -> DeclarationStatement | AssignmentStatement
    if (this->currentToken.type == Int || this->currentToken.type == Float || this->currentToken.type == Void) {
        this->DeclarationStatement();
    } else if (this->currentToken.type == Identifier) {
        this->AssignmentStatement();
    } else {
        std::cerr << "Error: Expected Declaration or Assignment Statement" << std::endl;
        exit(1);
    }
}

void Parser::DeclarationStatement() {
    // DeclarationStatement -> VariableType DeclarationVariableList ;
    this->VariableType();
    this->DeclarationVariableList();
    if (this->currentToken.type == SEMICOLON) {
        this->getNextToken();
    } else {
        std::cerr << "Error: Expected Semicolon" << std::endl;
        exit(1);
    }
}

void Parser::VariableType() {
    // VariableType -> Int | Float | Void
    if (this->currentToken.type == Int) {
        this->getNextToken();
    } else if (this->currentToken.type == Float) {
        this->getNextToken();
    } else if (this->currentToken.type == Void) {
        this->getNextToken();
    } else {
        std::cerr << "Error: Expected Int, Float or Void" << std::endl;
        exit(1);
    }
}

void Parser::DeclarationVariableList() {
    // DeclarationVariableList -> DeclarationVariable | DeclarationVariableList , DeclarationVariable
    this->DeclarationVariable();
    if (this->currentToken.type == COMMA) {
        this->getNextToken();
        this->DeclarationVariableList();
    }
}

void Parser::DeclarationVariable() {
    // DeclarationVariable -> Identifier | Identifier ASSIGN Expression
    if (this->currentToken.type == Identifier) {
        this->getNextToken();
        if (this->currentToken.type == ASSIGN) {
            this->getNextToken();
            this->Expression();
        }
    } else {
        std::cerr << "Error: Expected Identifier" << std::endl;
        exit(1);
    }
}

void Parser::AssignmentStatement() {
    // AssignmentStatement -> Identifier = Expression ;
    if (this->currentToken.type == Identifier) {
        this->getNextToken();
        if (this->currentToken.type == ASSIGN) {
            this->getNextToken();
            this->Expression();
            if (this->currentToken.type == SEMICOLON) {
                this->getNextToken();
            } else {
                std::cerr << "Error: Expected Semicolon" << std::endl;
                exit(1);
            }
        } else {
            std::cerr << "Error: Expected Assignment Operator" << std::endl;
            exit(1);
        }
    } else {
        std::cerr << "Error: Expected Identifier" << std::endl;
        exit(1);
    }
}

void Parser::Expression() {
    // Expression -> Term | Expression + Term | Expression - Term
    this->Term();
    if (this->currentToken.type == Plus) {
        this->getNextToken();
        this->Expression();
    } else if (this->currentToken.type == Minus) {
        this->getNextToken();
        this->Expression();
    }
}

void Parser::Term() {
    // Term -> Factor | Term * Factor | Term / Factor | Term % Factor
    this->Factor();
    if (this->currentToken.type == Multiply) {
        this->getNextToken();
        this->Term();
    } else if (this->currentToken.type == Divide) {
        this->getNextToken();
        this->Term();
    } else if (this->currentToken.type == Modulus) {
        this->getNextToken();
        this->Term();
    }
}

void Parser::Factor() {
    // Factor -> ( Expression ) | IntValue | FloatValue | Identifier
    if (this->currentToken.type == LEFT_PAREN) {
        this->getNextToken();
        this->Expression();
        if (this->currentToken.type == RIGHT_PAREN) {
            this->getNextToken();
        } else {
            std::cerr << "Error: Expected Right Parenthesis" << std::endl;
            exit(1);
        }
    } else if (this->currentToken.type == IntValue) {
        this->getNextToken();
    } else if (this->currentToken.type == FloatValue) {
        this->getNextToken();
    } else if (this->currentToken.type == Identifier) {
        this->getNextToken();
    } else {
        std::cerr << "Error: Expected Left Parenthesis, IntValue, FloatValue or Identifier" << std::endl;
        exit(1);
    }
}

void Parser::startParse() {
    // Globalstatments -> Globalstatment Globalstatments | £`
    // Globalstatment -> DeclarationStatement | AssignmentStatement
    // DeclarationStatement -> VariableType DeclarationVariableList ;
    // VariableType -> Int | Float | Void
    // DeclarationVariableList -> DeclarationVariable | DeclarationVariableList , DeclarationVariable
    // DeclarationVariable -> Identifier | Identifier ASSIGN Expression
    // AssignmentStatement -> Identifier = Expression ;
    // Expression -> Term | Expression + Term | Expression - Term
    // Term -> Factor | Term * Factor | Term / Factor | Term % Factor
    // Factor -> ( Expression ) | IntValue | FloatValue | Identifier

    this->getNextToken();
    GlobalStatements();
}