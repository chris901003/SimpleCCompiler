#include <vector>
#include <iostream>
#include "./parser.h"
#include "../LLVMController/llvm_controller.h"

Parser::Parser(std::vector<Token> tokens) {
    this->tokens = tokens;
    this->tokenIndex = 0;
    this->llvmController = new LLVMController();
}

void Parser::getNextToken() {
    this->currentToken = this->tokens[this->tokenIndex++];
}

void Parser::getPrevToken() {
    this->tokenIndex--;
    this->currentToken = this->tokens[this->tokenIndex - 1];
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
    if (this->currentToken.type == Int || this->currentToken.type == Void) {
        this->FunctionDefinition();
    } else {
        std::cerr << "GlobalStatement Error: Expected Declaration, Assignment or Function Definition" << std::endl;
        exit(1);
    }
}

void Parser::DeclarationExpression() {
    // DeclarationExpression -> VariableType DeclarationVariableList
    this->VariableType();
    this->DeclarationVariableList();
}

void Parser::DeclarationStatement() {
    // DeclarationStatement -> DeclarationExpression ;
    this->DeclarationExpression();
    if (this->currentToken.type == SEMICOLON) {
        this->getNextToken();
    } else {
        std::cerr << "DeclarationStatement Error: Expected Semicolon" << std::endl;
        exit(1);
    }
}

void Parser::VariableType() {
    // VariableType -> Int | Void
    llvmController->variableType = this->currentToken.type;
    if (this->currentToken.type == Int) {
        this->getNextToken();
    } else if (this->currentToken.type == Void) {
        this->getNextToken();
    } else {
        std::cerr << "VariableType Error: Expected Int or Void" << std::endl;
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
        this->llvmController->variableName = this->currentToken.sValue;
        this->llvmController->createVariable();
        this->getNextToken();
        if (this->currentToken.type == ASSIGN) {
            this->getNextToken();
            this->Expression();
            this->llvmController->assignVariable();
        }
    } else {
        std::cerr << "DeclarationVariable Error: Expected Identifier" << std::endl;
        exit(1);
    }
}

void Parser::AssignmentExpression() {
    // AssignmentExpression -> Identifier = Expression
    if (this->currentToken.type == Identifier) {
        this->llvmController->variableName = this->currentToken.sValue;
        this->getNextToken();
        if (this->currentToken.type == ASSIGN) {
            this->getNextToken();
            this->Expression();
            this->llvmController->assignVariable();
        } else {
            std::cerr << "AssignmentExpression Error: Expected Assignment Operator" << std::endl;
            exit(1);
        }
    } else {
        std::cerr << "AssignmentExpression Error: Expected Identifier" << std::endl;
        exit(1);
    }
}

void Parser::AssignmentStatement() {
    // AssignmentStatement -> AssignmentExpression ;
    this->AssignmentExpression();
    if (this->currentToken.type == SEMICOLON) {
        this->getNextToken();
    } else {
        std::cerr << "AssignmentStatement Error: Expected Semicolon" << std::endl;
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
    // Statement -> DeclarationStatement | AssignmentStatement | CallFunctionStatement | IfStatement | WhileStatement | ForStatement | £`
    if (this->currentToken.type == Int || this->currentToken.type == Void) {
        this->DeclarationStatement();
    } else if (this->currentToken.type == Identifier) {
        this->getNextToken();
        if (this->currentToken.type == LEFT_PAREN) {
            this->getPrevToken();
            this->CallFunctionStatement();
        } else {
            this->getPrevToken();
            this->AssignmentStatement();
        }
    } else if (this->currentToken.type == IF) {
        this->IfStatement();
    } else if (this->currentToken.type == WHILE) {
        this->WhileStatement();
    } else if (this->currentToken.type == FOR) {
        this->ForStatement();
    }
}

void Parser::FunctionDefinition() {
    // FunctionDefinition -> VariableType Identifier ( Parameters ) FunctionBlock
    this->llvmController->functionReturnType = this->currentToken.type;
    this->VariableType();
    if (this->currentToken.type == Identifier) {
        this->llvmController->functionName = this->currentToken.sValue;
        this->llvmController->definitionParameters = {};
        this->getNextToken();
        if (this->currentToken.type == LEFT_PAREN) {
            this->getNextToken();
            this->Parameters();
            if (this->currentToken.type == RIGHT_PAREN) {
                this->llvmController->createFunctionDefinition();
                this->getNextToken();
                this->FunctionBlock();
            } else {
                std::cerr << "FunctionDefinition Error: Expected Right Parenthesis" << std::endl;
                exit(1);
            }
        } else {
            std::cerr << "FunctionDefinition Error: Expected Left Parenthesis" << std::endl;
            exit(1);
        }
    } else {
        std::cerr << "FunctionDefinition Error: Expected Identifier" << std::endl;
        exit(1);
    }
}

void Parser::Parameters() {
    // Parameters -> £` | ParameterList
    if (this->currentToken.type == Int) {
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
        this->llvmController->definitionParameters.push_back({this->currentToken.sValue, this->llvmController->variableType});
        this->getNextToken();
    } else {
        std::cerr << "Parameter Error: Expected Identifier" << std::endl;
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
            std::cerr << "FunctionBlock Error: Expected Right Brace" << std::endl;
            exit(1);
        }
    } else {
        std::cerr << "FunctionBlock Error: Expected Left Brace" << std::endl;
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
                std::cerr << "ReturnStatement Error: Expected Semicolon" << std::endl;
                exit(1);
            }
        }
    } else {
        std::cerr << "ReturnStatement Error: Expected Return Statement" << std::endl;
        exit(1);
    }
}

void Parser::LoopBlockStatements() {
    // LoopBlockStatements -> LoopBlockStatement LoopBlockStatements | £`
    this->LoopBlockStatement();
    if (this->currentToken.type != RIGHT_BRACE) {
        this->LoopBlockStatements();
    }
}

void Parser::LoopBlockStatement() {
    // LoopBlockStatement -> Statement | ReturnStatement | FlowBreakStatement
    if (this->currentToken.type == RETURN) {
        this->ReturnStatement();
    } else if (this->currentToken.type == BREAK || this->currentToken.type == CONTINUE) {
        this->FlowBreakStatement();
    } else {
        this->Statement();
    }
}

void Parser::FlowBreakStatement() {
    // FlowBreakStatement -> break ; | continue ;
    if (this->currentToken.type == BREAK) {
        this->getNextToken();
        if (this->currentToken.type == SEMICOLON) {
            this->getNextToken();
        } else {
            std::cerr << "FlowBreakStatement Error: Expected Semicolon" << std::endl;
            exit(1);
        }
    } else if (this->currentToken.type == CONTINUE) {
        this->getNextToken();
        if (this->currentToken.type == SEMICOLON) {
            this->getNextToken();
        } else {
            std::cerr << "FlowBreakStatement Error: Expected Semicolon" << std::endl;
            exit(1);
        }
    } else {
        std::cerr << "FlowBreakStatement Error: Expected Break or Continue Statement" << std::endl;
        exit(1);
    }
}

void Parser::Block() {
    // Block -> { FunctionBlockStatements } | FunctionBlockStatement
    if (this->currentToken.type == LEFT_BRACE) {
        this->getNextToken();
        this->FunctionBlockStatements();
        if (this->currentToken.type == RIGHT_BRACE) {
            this->getNextToken();
        } else {
            std::cerr << "Block Error: Expected Right Brace" << std::endl;
            exit(1);
        }
    } else {
        this->FunctionBlockStatement();    
    }
}

void Parser::LoopBlock() {
    // LoopBlock -> { LoopBlockStatements } | LoopBlockStatement
    if (this->currentToken.type == LEFT_BRACE) {
        this->getNextToken();
        this->LoopBlockStatements();
        if (this->currentToken.type == RIGHT_BRACE) {
            this->getNextToken();
        } else {
            std::cerr << "LoopBlock Error: Expected Right Brace" << std::endl;
            exit(1);
        }
    } else {
        this->LoopBlockStatement();
    }
}

void Parser::ForInitExpression() {
    // ForInitExpression -> AssignmentExpression | DeclarationExpression
    if (this->currentToken.type == Int || this->currentToken.type == Void) {
        this->DeclarationExpression();
    } else if (this->currentToken.type == Identifier) {
        this->AssignmentExpression();
    } else {
        std::cerr << "ForInitExpression Error: Expected Declaration or Assignment Expression" << std::endl;
        exit(1);
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
                std::cerr << "IfStatement Error: Expected Right Parenthesis" << std::endl;
                exit(1);
            }
        } else {
            std::cerr << "IfStatement Error: Expected Left Parenthesis" << std::endl;
            exit(1);
        }
    } else {
        std::cerr << "IfStatement Error: Expected If Statement" << std::endl;
        exit(1);
    }
}

void Parser::WhileStatement() {
    // WhileStatement -> while ( ConditionExpression ) LoopBlock
    if (this->currentToken.type == WHILE) {
        this->getNextToken();
        if (this->currentToken.type == LEFT_PAREN) {
            this->getNextToken();
            this->ConditionExpression();
            if (this->currentToken.type == RIGHT_PAREN) {
                this->getNextToken();
                this->LoopBlock();
            } else {
                std::cerr << "WhileStatement Error: Expected Right Parenthesis" << std::endl;
                exit(1);
            }
        } else {
            std::cerr << "WhileStatement Error: Expected Left Parenthesis" << std::endl;
            exit(1);
        }
    } else {
        std::cerr << "WhileStatement Error: Expected While Statement" << std::endl;
        exit(1);
    }
}

void Parser::ForStatement() {
    // ForStatement -> for ( ForInitExpression ; ConditionExpression ; AssignmentExpression ) LoopBlock
    if (this->currentToken.type == FOR) {
        this->getNextToken();
        if (this->currentToken.type == LEFT_PAREN) {
            this->getNextToken();
            this->ForInitExpression();
            if (this->currentToken.type == SEMICOLON) {
                this->getNextToken();
                this->ConditionExpression();
                if (this->currentToken.type == SEMICOLON) {
                    this->getNextToken();
                    this->AssignmentExpression();
                    if (this->currentToken.type == RIGHT_PAREN) {
                        this->getNextToken();
                        this->LoopBlock();
                    } else {
                        std::cerr << "ForStatement Error: Expected Right Parenthesis" << std::endl;
                        exit(1);
                    }
                } else {
                    std::cerr << "ForStatement Error: Expected Semicolon" << std::endl;
                    exit(1);
                }
            } else {
                std::cerr << "ForStatement Error: Expected Semicolon" << std::endl;
                exit(1);
            }
        } else {
            std::cerr << "ForStatement Error: Expected Left Parenthesis" << std::endl;
            exit(1);
        }
    } else {
        std::cerr << "ForStatement Error: Expected For Statement" << std::endl;
        exit(1);
    }
}

void Parser::CallFunctionExpression() {
    // CallFunctionExpression -> Identifier ( CallFunctionParameters )
    if (this->currentToken.type == Identifier) {
        this->getNextToken();
        if (this->currentToken.type == LEFT_PAREN) {
            this->getNextToken();
            this->CallFunctionParameters();
            if (this->currentToken.type == RIGHT_PAREN) {
                this->getNextToken();
            } else {
                std::cerr << "CallFunctionExpression Error: Expected Right Parenthesis" << std::endl;
                exit(1);
            }
        } else {
            std::cerr << "CallFunctionExpression Error: Expected Left Parenthesis" << std::endl;
            exit(1);
        }
    } else {
        std::cerr << "CallFunctionExpression Error: Expected Identifier" << std::endl;
        exit(1);
    }
}

void Parser::CallFunctionStatement() {
    // CallFunctionStatement -> CallFunctionExpression ;
    this->CallFunctionExpression();
    if (this->currentToken.type == SEMICOLON) {
        this->getNextToken();
    } else {
        std::cerr << "CallFunctionStatement Error: Expected Semicolon" << std::endl;
        exit(1);
    }
}

void Parser::CallFunctionParameters() {
    // CallFunctionParameters -> CallFunctionParameters | Expression | £`
    if (this->currentToken.type == RIGHT_PAREN) {
        return;
    }
    this->Expression();
    if (this->currentToken.type == COMMA) {
        this->getNextToken();
        this->CallFunctionParameters();
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
        this->llvmController->pushOperationStack('+');
        this->Expression();
    } else if (this->currentToken.type == Minus) {
        this->getNextToken();
        this->llvmController->pushOperationStack('-');
        this->Expression();
    }
}

void Parser::Term() {
    // Term -> Factor | Term * Factor | Term / Factor | Term % Factor
    this->Factor();
    if (this->currentToken.type == Multiply) {
        this->getNextToken();
        this->llvmController->pushOperationStack('*');
        this->Term();
    } else if (this->currentToken.type == Divide) {
        this->getNextToken();
        this->llvmController->pushOperationStack('/');
        this->Term();
    } else if (this->currentToken.type == Modulus) {
        this->getNextToken();
        this->llvmController->pushOperationStack('%');
        this->Term();
    }
}

void Parser::Factor() {
    // Factor -> IntValue | Identifier | CallFunctionExpression
    if (this->currentToken.type == IntValue) {
        this->llvmController->pushIntValueStack(this->currentToken.iValue);
        this->getNextToken();
    } else if (this->currentToken.type == Identifier) {
        string variableName = this->currentToken.sValue;
        this->getNextToken();
        if (this->currentToken.type == LEFT_PAREN) {
            this->getPrevToken();
            this->CallFunctionExpression();
        }
        this->llvmController->pushVariableToValueStack(variableName);
    } else {
        std::cerr << "Error: Expected Expression" << std::endl;
        exit(1);
    }
}

void Parser::startParse() {
    // Globalstatments -> Globalstatment Globalstatments | £`
    // Globalstatment -> FunctionDefinition

    // DeclarationExpression -> VariableType DeclarationVariableList
    // DeclarationStatement -> DeclarationExpression ;
    // VariableType -> Int | Void
    // DeclarationVariableList -> DeclarationVariable | DeclarationVariableList , DeclarationVariable
    // DeclarationVariable -> Identifier | Identifier ASSIGN Expression

    // AssignmentExpression -> Identifier = Expression
    // AssignmentStatement -> AssignmentExpression ;

    // Statements -> Statement Statements | £`
    // Statement -> DeclarationStatement | AssignmentStatement | CallFunctionStatement | IfStatement | WhileStatement | ForStatement | £`

    // FunctionDefinition -> VariableType Identifier ( Parameters ) FunctionBlock
    // Parameters -> £` | ParameterList
    // ParameterList -> Parameter | ParameterList , Parameter
    // Parameter -> VariableType Identifier
    // FunctionBlock -> { FunctionBlockStatements }
    // FunctionBlockStatements -> FunctionBlockStatement FunctionBlockStatements | £`
    // FunctionBlockStatement -> Statement | ReturnStatement
    // ReturnStatement -> retrun ; | return Expression ;

    // LoopBlockStatements -> LoopBlockStatement LoopBlockStatements | £`
    // LoopBlockStatement -> Statement | ReturnStatement | FlowBreakStatement
    // FlowBreakStatement -> break ; | continue ;

    // Block -> { FunctionBlockStatements } | FunctionBlockStatement
    // LoopBlock -> { LoopBlockStatements } | LoopBlockStatement

    // ForInitExpression -> AssignmentExpression | DeclarationExpression

    // IfStatement -> if ( ConditionExpression ) Block | if ( ConditionExpression ) Block else Block
    // WhileStatement -> while ( ConditionExpression ) LoopBlock
    // ForStatement -> for ( ForInitExpression ; ConditionExpression ; AssignmentExpression ) LoopBlock

    // CallFunctionExpression -> Identifier ( CallFunctionParameters )
    // CallFunctionStatement -> CallFunctionExpression ;
    // CallFunctionParameters -> CallFunctionParameters | Expression | £`

    // ConditionExpression -> Expression < Expression | Expression > Expression | Expression <= Expression | Expression >= Expression | Expression == Expression | Expression != Expression

    // Expression -> Term | Expression + Term | Expression - Term
    // Term -> Factor | Term * Factor | Term / Factor | Term % Factor
    // Factor -> IntValue | Identifier | CallFunctionExpression

    this->getNextToken();
    GlobalStatements();

    std::cout << std::endl;
    std::cout << "====== LLVM IR =====" << std::endl;
    llvmController->showLLVMCode();
}