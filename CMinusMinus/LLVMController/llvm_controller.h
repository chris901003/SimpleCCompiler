#ifndef LLVM_CONTROLLER_H
#define LLVM_CONTROLLER_H

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <vector>
#include <string>
#include <utility>
#include <stack>
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
        int operationPriority(char operation);
        bool variableIsExist(Function *function, string &name);
        AllocaInst* findAllocaByName(Function* function, string& variableName);

    public:
        LLVMController();

        TokenType variableType;
        string variableName;
        stack<Value*> intValueStack;
        stack<char> operationStack;

        TokenType functionReturnType;
        string functionName;
        vector<pair<string, TokenType>> definitionParameters;

        void showLLVMCode();
        void createFunctionDefinition();
        void createVariable();
        void assignVariable();
        void pushIntValueStack(int value);
        void pushVariableToValueStack(string variableName);
        void pushOperationStack(char operation);
        void createAddition();
        void createSubtraction();
        void createMultiplication();
        void createDivision();
        void createModulo();
        void createReturnWithoutValue();
        void createReturnWithValue();
};

#endif