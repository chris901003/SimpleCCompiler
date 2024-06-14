#include "./llvm_controller.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <vector>
#include <iostream>

using namespace llvm;
using namespace std;

LLVMController::LLVMController() {
    context = new LLVMContext();
    module = new Module("C-- Compiler", *context);
    builder = new IRBuilder<>(*context);
}

void LLVMController::showLLVMCode() {
    module->print(errs(), nullptr);
}

Type* LLVMController::getIntType() {
    return Type::getInt32Ty(*context);
}

Type* LLVMController::getFloatType() {
    return Type::getFloatTy(*context);
}

int LLVMController::operationPriority(char operation) {
    if (operation == '+' || operation == '-') {
        return 1;
    } else if (operation == '*' || operation == '/' || operation == '%') {
        return 2;
    }
    return 0;
}

bool LLVMController::variableIsExist(Function *function, string &name) {
    for (auto &bb : *function) {
        for (auto &inst : bb) {
            if (AllocaInst *allocaInst = dyn_cast<AllocaInst>(&inst)) {
                if (allocaInst->getName() == name) {
                    return true;
                }
            }
        }
    }
    return false;
}

void LLVMController::createFunctionDefinition() {
    vector<Type*> parametersType;
    for (pair<string, TokenType> parameter : definitionParameters) {
        if (parameter.second == TokenType::Int) {
            parametersType.push_back(getIntType());
        }
    }

    Type* returnType;
    if (functionReturnType == TokenType::Int) {
        returnType = getIntType();
    } else if (functionReturnType == TokenType::Void) {
        returnType = builder->getVoidTy();
    }

    FunctionType *functionType = FunctionType::get(returnType, parametersType, false);
    Function *function = Function::Create(functionType, Function::ExternalLinkage, functionName, module);
    int idx = 0;
    for (Function::arg_iterator arg = function->arg_begin(); arg != function->arg_end(); ++arg, ++idx) {
        arg->setName(definitionParameters[idx].first);
    }

    BasicBlock *basicBlock = BasicBlock::Create(*context, "entry", function);
    builder->SetInsertPoint(basicBlock);
}

AllocaInst* LLVMController::findAllocaByName(Function* function, string& variableName) {
    for (auto& BB : *function) {
        for (auto& I : BB) {
            if (llvm::AllocaInst* AI = llvm::dyn_cast<llvm::AllocaInst>(&I)) {
                if (AI->getName() == variableName) {
                    return AI;
                }
            }
        }
    }
    return nullptr;
}

void LLVMController::createVariable() {
    AllocaInst *alloca;
    if (variableType == TokenType::Int) {
        if (!this->variableIsExist(builder->GetInsertBlock()->getParent(), variableName)) {
            alloca = builder->CreateAlloca(getIntType(), nullptr, variableName);
        } else {
            cout << "Variable " << variableName << " already exists" << endl;
            exit(1);
        }
    }
}

void LLVMController::assignVariable() {
    AllocaInst* variable = this->findAllocaByName(builder->GetInsertBlock()->getParent(), variableName);
    if (variable == nullptr) {
        cout << "Variable " << variableName << " not found" << endl;
        exit(1);
    }
    while (this->operationStack.size() > 0) {
        char lastOperation = this->operationStack.top();
        this->operationStack.pop();
        switch (lastOperation) {
            case '+':
                this->createAddition();
                break;
            case '-':
                this->createSubtraction();
                break;
            case '*':
                this->createMultiplication();
                break;
            case '/':
                this->createDivision();
                break;
            case '%':
                this->createModulo();
                break;
        }
    }
    Value *value = intValueStack.top();
    intValueStack.pop();
    builder->CreateStore(value, variable);
}

void LLVMController::pushIntValueStack(int value) {
    Value *intValue = ConstantInt::get(*context, APInt(32, value));
    intValueStack.push(intValue);
}

void LLVMController::pushVariableToValueStack(string variableName) {
    AllocaInst* variable = this->findAllocaByName(builder->GetInsertBlock()->getParent(), variableName);
    if (variable == nullptr) {
        cout << "Variable " << variableName << " not found" << endl;
        exit(1);
    }
    intValueStack.push(variable);
}

void LLVMController::pushOperationStack(char operation) {
    int currentOperationPriority = this->operationPriority(operation);
    while (this->operationStack.size() > 0) {
        char lastOperation = this->operationStack.top();
        if (this->operationPriority(lastOperation) >= currentOperationPriority) {
            this->operationStack.pop();
            switch (lastOperation) {
                case '+':
                    this->createAddition();
                    break;
                case '-':
                    this->createSubtraction();
                    break;
                case '*':
                    this->createMultiplication();
                    break;
                case '/':
                    this->createDivision();
                    break;
                case '%':
                    this->createModulo();
                    break;
            }
        } else {
            break;
        }
    }
    this->operationStack.push(operation);
}

void LLVMController::createAddition() {
    Value *lhs;
    Value *rhs;
    rhs = intValueStack.top();
    intValueStack.pop();
    lhs = intValueStack.top();
    intValueStack.pop();
    Value *addition = builder->CreateAdd(lhs, rhs, "addition");
    intValueStack.push(addition);
}

void LLVMController::createSubtraction() {
    Value *lhs;
    Value *rhs;
    rhs = intValueStack.top();
    intValueStack.pop();
    lhs = intValueStack.top();
    intValueStack.pop();
    Value *subtraction = builder->CreateSub(lhs, rhs, "subtraction");
    intValueStack.push(subtraction);
}

void LLVMController::createMultiplication() {
    Value *lhs;
    Value *rhs;
    rhs = intValueStack.top();
    intValueStack.pop();
    lhs = intValueStack.top();
    intValueStack.pop();
    Value *multiplication = builder->CreateMul(lhs, rhs, "multiplication");
    intValueStack.push(multiplication);
}

void LLVMController::createDivision() {
    Value *lhs;
    Value *rhs;
    rhs = intValueStack.top();
    intValueStack.pop();
    lhs = intValueStack.top();
    intValueStack.pop();
    Value *division = builder->CreateSDiv(lhs, rhs, "division");
    intValueStack.push(division);
}

void LLVMController::createModulo() {
    Value *lhs;
    Value *rhs;
    rhs = intValueStack.top();
    intValueStack.pop();
    lhs = intValueStack.top();
    intValueStack.pop();
    Value *modulo = builder->CreateSRem(lhs, rhs, "modulo");
    intValueStack.push(modulo);
}
