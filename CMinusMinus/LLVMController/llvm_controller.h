#ifndef LLVM_CONTROLLER_H
#define LLVM_CONTROLLER_H

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <vector>
#include <string>
#include <utility>
#include <stack>
#include <map>
#include "../Token/token.h"

using namespace llvm;
using namespace std;

class LLVMController {
    private:
        LLVMContext *context;
        Module *module;
        IRBuilder<> *builder;
        Function *printfFunc;
        Type* getIntType();
        Type* getFloatType();
        Type* getBoolType();
        int operationPriority(char operation);
        bool variableIsExist(Function *function, string &name);
        AllocaInst* findAllocaByName(Function* function, string& variableName);
        void calIntValueStack();
        void createVariableIfNeeded(Function* function, string& variableName, Type* variableType);
        string leftConditionKey = "left_condition_value";
        string rightConditionKey = "right_condition_value";
        string conditionResultKey = "condition_result";
        stack<BasicBlock*> elseBlockStack;
        stack<BasicBlock*> mergeBlockStack;

        stack<bool> isInWhileBlockStack;

        stack<BasicBlock*> whileConditionBlockStack;
        stack<BasicBlock*> whileBodyBlockStack;
        stack<BasicBlock*> whileAfterBlockStack;

        stack<BasicBlock*> forConditionBlockStack;
        stack<BasicBlock*> forExpressionBlockStack;
        stack<BasicBlock*> forBodyBlockStack;
        stack<BasicBlock*> forAfterBlockStack;

    public:
        LLVMController();

        TokenType variableType;
        string variableName;
        stack<stack<Value*>> intValueStack;
        stack<stack<char>> operationStack;

        TokenType functionReturnType;
        string functionName;
        vector<pair<string, TokenType>> definitionParameters;

        string callFunctionName;
        vector<Value*> callFunctionParameters;
        TokenType conditionType;

        void showLLVMCode();
        void createFunctionDeclarationIfNeeded();
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
        void createPrintFunction();
        void callPrintFunction();
        void createCallFunction();
        void moveIntValueStackToCallFunctionParameters();
        void saveConditionValue(bool isLeft);
        void calCondition();
        void createIfStatement();
        void changeToElseBlock();
        void changeToMergeBlock();
        void createWhileStatement();
        void createWhileConditionJump();
        void createJumpToWhileCondition();
        void createForStatement();
        void createJumpToForCondition();
        void createForConditionJump();
        void createJumpToForExpression();
        void createJumpToForBody();
        void createForBodyJumpBackToCondition();
        void createContinueJump();
        void createBreakJump();
};

#endif