#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "../Token/token.h"
#include "../LLVMController/llvm_controller.h"

class Parser {
    private:
        Token currentToken;
        int tokenIndex;
        LLVMController *llvmController;

        void getNextToken();
        void getPrevToken();

        void GlobalStatements();
        void GlobalStatement();

        void DeclarationExpression();
        void DeclarationStatement();
        void VariableType();
        void DeclarationVariableList();
        void DeclarationVariable();

        void AssignmentExpression();
        void AssignmentStatement();

        void Statements();
        void Statement();

        void FunctionDefinition();
        void Parameters();
        void ParameterList();
        void Parameter();
        void FunctionBlock();
        void FunctionBlockStatements();
        void FunctionBlockStatement();
        void ReturnStatement();

        void LoopBlockStatements();
        void LoopBlockStatement();
        void FlowBreakStatement();

        void Block();
        void LoopBlock();

        void ForInitExpression();

        void IfStatement();
        void WhileStatement();
        void ForStatement();

        void CallFunctionExpression();
        void CallFunctionStatement();
        void CallFunctionParameters();

        void ConditionExpression();

        void Expression();
        void Term();
        void Factor();

    public:
        Parser(std::vector<Token>);
        std::vector<Token>tokens;
        void startParse();
};

#endif