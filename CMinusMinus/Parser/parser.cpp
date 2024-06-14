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
    // Globalstatment -> FunctionDefinition
    if (this->currentToken.type == Int || this->currentToken.type == Float || this->currentToken.type == Void) {
        this->FunctionDefinition();
    } else {
        std::cerr << "Error: Expected Declaration, Assignment or Function Definition" << std::endl;
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

void Parser::Statements() {
    // Statements -> Statement Statements | £`
    this->Statement();
    if (this->currentToken.type != RIGHT_BRACE) {
        this->Statements();
    }
}

void Parser::Statement() {
    // Statement -> DeclarationStatement | AssignmentStatement | IfStatement | WhileStatement | £`
    if (this->currentToken.type == Int || this->currentToken.type == Float || this->currentToken.type == Void) {
        this->DeclarationStatement();
    } else if (this->currentToken.type == Identifier) {
        this->AssignmentStatement();
    } else if (this->currentToken.type == IF) {
        this->IfStatement();
    } else if (this->currentToken.type == WHILE) {
        this->WhileStatement();
    }
}

void Parser::FunctionDefinition() {
    // FunctionDefinition -> VariableType Identifier ( Parameters ) FunctionBlock
    this->VariableType();
    if (this->currentToken.type == Identifier) {
        this->getNextToken();
        if (this->currentToken.type == LEFT_PAREN) {
            this->getNextToken();
            this->Parameters();
            if (this->currentToken.type == RIGHT_PAREN) {
                this->getNextToken();
                this->FunctionBlock();
            } else {
                std::cerr << "Error: Expected Right Parenthesis" << std::endl;
                exit(1);
            }
        } else {
            std::cerr << "Error: Expected Left Parenthesis" << std::endl;
            exit(1);
        }
    } else {
        std::cerr << "Error: Expected Identifier" << std::endl;
        exit(1);
    }
}

void Parser::Parameters() {
    // Parameters -> £` | ParameterList
    if (this->currentToken.type == Int || this->currentToken.type == Float) {
        this->ParameterList();
    }
}

void Parser::ParameterList() {
    // ParameterList -> Parameter | ParameterList , Parameter
    this->Parameter();
    if (this->currentToken.type == COMMA) {
        this->getNextToken();
        this->ParameterList();
    }
}

void Parser::Parameter() {
    // Parameter -> VariableType Identifier
    this->VariableType();
    if (this->currentToken.type == Identifier) {
        this->getNextToken();
    } else {
        std::cerr << "Error: Expected Identifier" << std::endl;
        exit(1);
    }
}

void Parser::FunctionBlock() {
    // FunctionBlock -> { FunctionBlockStatements }
    if (this->currentToken.type == LEFT_BRACE) {
        this->getNextToken();
        this->FunctionBlockStatements();
        if (this->currentToken.type == RIGHT_BRACE) {
            this->getNextToken();
        } else {
            std::cerr << "Error: Expected Right Brace" << std::endl;
            exit(1);
        }
    } else {
        std::cerr << "Error: Expected Left Brace" << std::endl;
        exit(1);
    }
}

void Parser::FunctionBlockStatements() {
    // FunctionBlockStatements -> FunctionBlockStatement FunctionBlockStatements | £`
    this->FunctionBlockStatement();
    if (this->currentToken.type != RIGHT_BRACE) {
        this->FunctionBlockStatements();
    }
}

void Parser::FunctionBlockStatement() {
    // FunctionBlockStatement -> Statement | ReturnStatement
    if (this->currentToken.type == RETURN) {
        this->ReturnStatement();
    } else {
        this->Statement();
    }
}

void Parser::ReturnStatement() {
    // ReturnStatement -> retrun ; | return Expression ;
    if (this->currentToken.type == RETURN) {
        this->getNextToken();
        if (this->currentToken.type == SEMICOLON) {
            this->getNextToken();
        } else {
            this->Expression();
            if (this->currentToken.type == SEMICOLON) {
                this->getNextToken();
            } else {
                std::cerr << "Error: Expected Semicolon" << std::endl;
                exit(1);
            }
        }
    } else {
        std::cerr << "Error: Expected Return Statement" << std::endl;
        exit(1);
    }
}

void Parser::Block() {
    // Block -> { Statements } | Statement
    if (this->currentToken.type == LEFT_BRACE) {
        this->getNextToken();
        this->Statements();
        if (this->currentToken.type == RIGHT_BRACE) {
            this->getNextToken();
        } else {
            std::cerr << "Error: Expected Right Brace" << std::endl;
            exit(1);
        }
    } else {
        this->Statement();    
    }
}

void Parser::IfStatement() {
    // IfStatement -> if ( ConditionExpression ) Block | if ( ConditionExpression ) Block else Block
    if (this->currentToken.type == IF) {
        this->getNextToken();
        if (this->currentToken.type == LEFT_PAREN) {
            this->getNextToken();
            this->ConditionExpression();
            if (this->currentToken.type == RIGHT_PAREN) {
                this->getNextToken();
                this->Block();
                if (this->currentToken.type == ELSE) {
                    this->getNextToken();
                    this->Block();
                }
            } else {
                std::cerr << "Error: Expected Right Parenthesis" << std::endl;
                exit(1);
            }
        } else {
            std::cerr << "Error: Expected Left Parenthesis" << std::endl;
            exit(1);
        }
    } else {
        std::cerr << "Error: Expected If Statement" << std::endl;
        exit(1);
    }
}

void Parser::WhileStatement() {
    // WhileStatement -> while ( ConditionExpression ) Block
    if (this->currentToken.type == WHILE) {
        this->getNextToken();
        if (this->currentToken.type == LEFT_PAREN) {
            this->getNextToken();
            this->ConditionExpression();
            if (this->currentToken.type == RIGHT_PAREN) {
                this->getNextToken();
                this->Block();
            } else {
                std::cerr << "Error: Expected Right Parenthesis" << std::endl;
                exit(1);
            }
        } else {
            std::cerr << "Error: Expected Left Parenthesis" << std::endl;
            exit(1);
        }
    } else {
        std::cerr << "Error: Expected While Statement" << std::endl;
        exit(1);
    }
}

void Parser::ConditionExpression() {
    // ConditionExpression -> Expression < Expression | Expression > Expression | Expression <= Expression | Expression >= Expression | Expression == Expression | Expression != Expression
    this->Expression();
    if (this->currentToken.type == LessThan) {
        this->getNextToken();
        this->Expression();
    } else if (this->currentToken.type == GreaterThan) {
        this->getNextToken();
        this->Expression();
    } else if (this->currentToken.type == LessThanEqual) {
        this->getNextToken();
        this->Expression();
    } else if (this->currentToken.type == GreaterThanEqual) {
        this->getNextToken();
        this->Expression();
    } else if (this->currentToken.type == Equal) {
        this->getNextToken();
        this->Expression();
    } else if (this->currentToken.type == NotEqual) {
        this->getNextToken();
        this->Expression();
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
    // Globalstatment -> FunctionDefinition

    // DeclarationStatement -> VariableType DeclarationVariableList ;
    // VariableType -> Int | Float | Void
    // DeclarationVariableList -> DeclarationVariable | DeclarationVariableList , DeclarationVariable
    // DeclarationVariable -> Identifier | Identifier ASSIGN Expression

    // AssignmentStatement -> Identifier = Expression ;

    // Statements -> Statement Statements | £`
    // Statement -> DeclarationStatement | AssignmentStatement | IfStatement | WhileStatement | £`

    // FunctionDefinition -> VariableType Identifier ( Parameters ) FunctionBlock
    // Parameters -> £` | ParameterList
    // ParameterList -> Parameter | ParameterList , Parameter
    // Parameter -> VariableType Identifier
    // FunctionBlock -> { FunctionBlockStatements }
    // FunctionBlockStatements -> FunctionBlockStatement FunctionBlockStatements | £`
    // FunctionBlockStatement -> Statement | ReturnStatement
    // ReturnStatement -> retrun ; | return Expression ;

    // Block -> { Statements } | Statement

    // IfStatement -> if ( ConditionExpression ) Block | if ( ConditionExpression ) Block else Block
    // WhileStatement -> while ( ConditionExpression ) Block

    // ConditionExpression -> Expression < Expression | Expression > Expression | Expression <= Expression | Expression >= Expression | Expression == Expression | Expression != Expression

    // Expression -> Term | Expression + Term | Expression - Term
    // Term -> Factor | Term * Factor | Term / Factor | Term % Factor
    // Factor -> ( Expression ) | IntValue | FloatValue | Identifier

    this->getNextToken();
    GlobalStatements();
}