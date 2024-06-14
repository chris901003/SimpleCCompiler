#ifndef LLVM_CONTROLLER_H
#define LLVM_CONTROLLER_H

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <vector>
#include <string>
#include <utility>
#include "../Token/token.h"

using namespace llvm;
using namespace std;

class LLVMController {
    private:
        LLVMContext *context;
        Module *module;
        IRBuilder<> *builder;
        Type* getIntType();
        Type* getFloatType();

    public:
        LLVMController();

        TokenType variableType;

        TokenType functionReturnType;
        string functionName;
        vector<pair<string, TokenType>> definitionParameters;

        void showLLVMCode();
        void createFunctionDefinition();
};

#endif