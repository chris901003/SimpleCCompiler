#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "../Token/token.h"

class Parser {
    private:
        Token currentToken;
        int tokenIndex;
        void getNextToken();

        void GlobalStatements();
        void GlobalStatement();

        void DeclarationStatement();
        void VariableType();
        void DeclarationVariableList();
        void DeclarationVariable();

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

        void Block();

        void IfStatement();
        void WhileStatement();

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