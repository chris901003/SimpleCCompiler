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
        void Expression();
        void Term();
        void Factor();

    public:
        Parser(std::vector<Token>);
        std::vector<Token>tokens;
        void startParse();
};

#endif