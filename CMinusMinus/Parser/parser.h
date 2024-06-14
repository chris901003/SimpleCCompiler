#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "../Token/token.h"

class Parser {
    public:
        Parser(std::vector<Token>);
        std::vector<Token>tokens;
        void startParse();
};

#endif