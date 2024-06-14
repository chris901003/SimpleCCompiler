#include "./llvm_controller.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <vector>

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

void LLVMController::createFunctionDefinition() {
    vector<Type*> parametersType;
    for (pair<string, TokenType> parameter : definitionParameters) {
        if (parameter.second == TokenType::Int) {
            parametersType.push_back(getIntType());
        } else if (parameter.second == TokenType::Float) {
            parametersType.push_back(getFloatType());
        }
    }

    Type* returnType;
    if (functionReturnType == TokenType::Int) {
        returnType = getIntType();
    } else if (functionReturnType == TokenType::Float) {
        returnType = getFloatType();
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
