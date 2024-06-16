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
    std::error_code EC;
    llvm::raw_fd_ostream dest("a.ll", EC);
    if (EC) {
        llvm::errs() << "Could not open file: " << EC.message() << "\n";
        return;
    }
    module->print(dest, nullptr);
    dest.close();
    module->print(errs(), nullptr);
}

Type* LLVMController::getIntType() {
    return Type::getInt32Ty(*context);
}

Type* LLVMController::getFloatType() {
    return Type::getFloatTy(*context);
}

Type* LLVMController::getBoolType() {
    return Type::getInt1Ty(*context);
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

void LLVMController::createFunctionDeclarationIfNeeded() {
    Function *function = module->getFunction(functionName);
    if (function == nullptr) {
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
        function = Function::Create(functionType, Function::ExternalLinkage, functionName, module);
        int idx = 0;
        for (Function::arg_iterator arg = function->arg_begin(); arg != function->arg_end(); ++arg, ++idx) {
            arg->setName(definitionParameters[idx].first);
        }
    }
}

void LLVMController::createFunctionDefinition() {
    Function *function = module->getFunction(functionName);
    if (function == nullptr) {
        cout << "Function " << functionName << " not found" << endl;
        exit(1);
    }
    BasicBlock *basicBlock = BasicBlock::Create(*context, "entry", function);
    builder->SetInsertPoint(basicBlock);

    int idx = 0;
    for (pair<string, TokenType> parameter : definitionParameters) {
        if (parameter.second == TokenType::Int) {
            AllocaInst *alloca = builder->CreateAlloca(getIntType(), nullptr, parameter.first + "_");
            builder->CreateStore(function->getArg(idx++), alloca);
        }
    }
}

AllocaInst* LLVMController::findAllocaByName(Function* function, string& variableName) {
    for (auto& BB : *function) {
        for (auto& I : BB) {
            if (llvm::AllocaInst* AI = llvm::dyn_cast<llvm::AllocaInst>(&I)) {
                if (AI->getName() == variableName || AI->getName() == variableName + "_") {
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

void LLVMController::calIntValueStack() {
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
}

void LLVMController::createVariableIfNeeded(Function* function, string& variableName, Type* variableType) {
    if (!this->variableIsExist(function, variableName)) {
        builder->CreateAlloca(variableType, nullptr, variableName);
    }
}

void LLVMController::assignVariable() {
    AllocaInst* variable = this->findAllocaByName(builder->GetInsertBlock()->getParent(), variableName);
    if (variable == nullptr) {
        cout << "Variable " << variableName << " not found" << endl;
        exit(1);
    }
    this->calIntValueStack();
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
    Value *variableValue = builder->CreateLoad(getIntType(), variable);
    intValueStack.push(variableValue);
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

void LLVMController::createReturnWithoutValue() {
    builder->CreateRetVoid();
}

void LLVMController::createReturnWithValue() {
    this->calIntValueStack();
    Value *returnValue = intValueStack.top();
    intValueStack.pop();
    builder->CreateRet(returnValue);
}

void LLVMController::createPrintFunction() {
    // Create a pointer type to i8 (i8*)
    llvm::PointerType* int8PtrType = llvm::PointerType::get(llvm::Type::getInt8Ty(*context), 0);

    // Create the printf function type: int (i8*, ...)
    llvm::FunctionType* printfFuncType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*context),  // Return type: i32
        { int8PtrType },  // Argument types: i8*
        true  // Variadic function: printf is variadic
    );
    this->printfFunc = Function::Create(printfFuncType, Function::ExternalLinkage, "printf", *module);
}

void LLVMController::callPrintFunction() {
    llvm::Constant* formatStr = builder->CreateGlobalStringPtr("%d\n");
    this->calIntValueStack();
    llvm::Value* printfArgs[] = { formatStr, intValueStack.top() };
    intValueStack.pop();
    builder->CreateCall(printfFunc, printfArgs);
}

void LLVMController::createCallFunction() {
    Function *function = module->getFunction(callFunctionName);
    if (function == nullptr) {
        cout << "Function " << callFunctionName << " not found" << endl;
        exit(1);
    }
    Value* ret = builder->CreateCall(function, callFunctionParameters);
    if (function->getReturnType() == getIntType()) {
        intValueStack.push(ret);
    }
    callFunctionParameters.clear();
}

void LLVMController::moveIntValueStackToCallFunctionParameters() {
    this->calIntValueStack();
    callFunctionParameters.push_back(intValueStack.top());
    intValueStack.pop();
}

void LLVMController::saveConditionValue(bool isLeft) {
    this->calIntValueStack();
    Value *value = intValueStack.top();
    intValueStack.pop();
    string variableName = isLeft ? leftConditionKey : rightConditionKey;
    this->createVariableIfNeeded(builder->GetInsertBlock()->getParent(), variableName, getIntType());
    AllocaInst *alloca = this->findAllocaByName(builder->GetInsertBlock()->getParent(), variableName);
    builder->CreateStore(value, alloca);
}

void LLVMController::calCondition() {
    AllocaInst *leftConditionValue = this->findAllocaByName(builder->GetInsertBlock()->getParent(), leftConditionKey);
    AllocaInst *rightConditionValue = this->findAllocaByName(builder->GetInsertBlock()->getParent(), rightConditionKey);
    if (leftConditionValue == nullptr || rightConditionValue == nullptr) {
        cout << "Variable " << leftConditionKey << " or " << rightConditionKey << " not found" << endl;
        exit(1);
    }
    Value *leftValue = builder->CreateLoad(getIntType(), leftConditionValue);
    Value *rightValue = builder->CreateLoad(getIntType(), rightConditionValue);
    Value *result;
    switch (this->conditionType) {
        case TokenType::LessThan:
            result = builder->CreateICmpSLT(leftValue, rightValue);
            break;
        case TokenType::LessThanEqual:
            result = builder->CreateICmpSLE(leftValue, rightValue);
            break;
        case TokenType::GreaterThan:
            result = builder->CreateICmpSGT(leftValue, rightValue);
            break;
        case TokenType::GreaterThanEqual:
            result = builder->CreateICmpSGE(leftValue, rightValue);
            break;
        case TokenType::Equal:
            result = builder->CreateICmpEQ(leftValue, rightValue);
            break;
        case TokenType::NotEqual:
            result = builder->CreateICmpNE(leftValue, rightValue);
            break;
        default:
            cout << "Invalid condition type: " << this->conditionType << endl;
            exit(1);
    }
    this->createVariableIfNeeded(builder->GetInsertBlock()->getParent(), conditionResultKey, getBoolType());
    AllocaInst *alloca = this->findAllocaByName(builder->GetInsertBlock()->getParent(), conditionResultKey);
    builder->CreateStore(result, alloca);
}

void LLVMController::createIfStatement() {
    AllocaInst *conditionResult = this->findAllocaByName(builder->GetInsertBlock()->getParent(), conditionResultKey);
    if (conditionResult == nullptr) {
        cout << "Variable " << conditionResultKey << " not found" << endl;
        exit(1);
    }
    Value *conditionValue = builder->CreateLoad(getBoolType(), conditionResult);
    Function *function = builder->GetInsertBlock()->getParent();
    BasicBlock *thenBlock = BasicBlock::Create(*context, "then", function);
    BasicBlock *elseBlock = BasicBlock::Create(*context, "else", function);
    BasicBlock *mergeBlock = BasicBlock::Create(*context, "ifcont", function);
    elseBlockStack.push(elseBlock);
    mergeBlockStack.push(mergeBlock);

    builder->CreateCondBr(conditionValue, thenBlock, elseBlock);
    builder->SetInsertPoint(thenBlock);
}

void LLVMController::changeToElseBlock() {
    BasicBlock *elseBlock = elseBlockStack.top();
    elseBlockStack.pop();
    BasicBlock *mergeBlock = mergeBlockStack.top();
    BasicBlock *currentBlock = builder->GetInsertBlock();
    if (currentBlock->getTerminator() == nullptr) {
        builder->CreateBr(mergeBlock);
    }
    builder->SetInsertPoint(elseBlock);
}

void LLVMController::changeToMergeBlock() {
    BasicBlock *mergeBlock = mergeBlockStack.top();
    mergeBlockStack.pop();
    BasicBlock *currentBlock = builder->GetInsertBlock();
    if (currentBlock->getTerminator() == nullptr) {
        builder->CreateBr(mergeBlock);
    }
    builder->SetInsertPoint(mergeBlock);
}

void LLVMController::createWhileStatement() {
    Function *function = builder->GetInsertBlock()->getParent();
    BasicBlock* whileConditionBlock = BasicBlock::Create(*context, "whilecond", function);
    BasicBlock* whileBodyBlock = BasicBlock::Create(*context, "whilebody", function);
    BasicBlock* whileAfterBlock = BasicBlock::Create(*context, "whileafter", function);
    whileConditionBlockStack.push(whileConditionBlock);
    whileBodyBlockStack.push(whileBodyBlock);
    whileAfterBlockStack.push(whileAfterBlock);

    builder->CreateBr(whileConditionBlock);
    builder->SetInsertPoint(whileConditionBlock);
}

void LLVMController::createWhileConditionJump() {
        AllocaInst *conditionResult = this->findAllocaByName(builder->GetInsertBlock()->getParent(), conditionResultKey);
    if (conditionResult == nullptr) {
        cout << "Variable " << conditionResultKey << " not found" << endl;
        exit(1);
    }
    Value *conditionValue = builder->CreateLoad(getBoolType(), conditionResult);
    BasicBlock* whileBodyBlock = whileBodyBlockStack.top();
    whileBodyBlockStack.pop();
    BasicBlock* whileAfterBlock = whileAfterBlockStack.top();
    builder->CreateCondBr(conditionValue, whileBodyBlock, whileAfterBlock);
    builder->SetInsertPoint(whileBodyBlock);
}

void LLVMController::createJumpToWhileCondition() {
    BasicBlock* whileConditionBlock = whileConditionBlockStack.top();
    whileConditionBlockStack.pop();
    BasicBlock* whileAfterBlock = whileAfterBlockStack.top();
    whileAfterBlockStack.pop();
    BasicBlock* currentBlock = builder->GetInsertBlock();
    if (currentBlock->getTerminator() == nullptr) {
        builder->CreateBr(whileConditionBlock);
    }
    builder->SetInsertPoint(whileAfterBlock);
}

void LLVMController::createForStatement() {
    Function *function = builder->GetInsertBlock()->getParent();
    BasicBlock* forInitBlock = BasicBlock::Create(*context, "forinit", function);
    BasicBlock* forConditionBlock = BasicBlock::Create(*context, "forcond", function);
    BasicBlock* forBodyBlock = BasicBlock::Create(*context, "forbody", function);
    BasicBlock* forExpressionBlock = BasicBlock::Create(*context, "forexpr", function);
    BasicBlock* forAfterBlock = BasicBlock::Create(*context, "forafter", function);
    forConditionBlockStack.push(forConditionBlock);
    forExpressionBlockStack.push(forExpressionBlock);
    forBodyBlockStack.push(forBodyBlock);
    forAfterBlockStack.push(forAfterBlock);

    builder->CreateBr(forInitBlock);
    builder->SetInsertPoint(forInitBlock);
}

void LLVMController::createJumpToForCondition() {
    BasicBlock* forConditionBlock = forConditionBlockStack.top();
    builder->CreateBr(forConditionBlock);
    builder->SetInsertPoint(forConditionBlock);
}

void LLVMController::createForConditionJump() {
    AllocaInst *conditionResult = this->findAllocaByName(builder->GetInsertBlock()->getParent(), conditionResultKey);
    if (conditionResult == nullptr) {
        cout << "Variable " << conditionResultKey << " not found" << endl;
        exit(1);
    }
    Value *conditionValue = builder->CreateLoad(getBoolType(), conditionResult);
    BasicBlock* forBodyBlock = forBodyBlockStack.top();
    BasicBlock* forAfterBlock = forAfterBlockStack.top();
    builder->CreateCondBr(conditionValue, forBodyBlock, forAfterBlock);
}

void LLVMController::createJumpToForExpression() {
    BasicBlock* forExpressionBlock = forExpressionBlockStack.top();
    builder->SetInsertPoint(forExpressionBlock);
}

void LLVMController::createJumpToForBody() {
    BasicBlock* forConditionBlock = forConditionBlockStack.top();
    forConditionBlockStack.pop();
    BasicBlock* forBodyBlock = forBodyBlockStack.top();
    forBodyBlockStack.pop();
    builder->CreateBr(forConditionBlock);
    builder->SetInsertPoint(forBodyBlock);
}

void LLVMController::createForBodyJumpBackToCondition() {
    BasicBlock* forExpressionBlock = forExpressionBlockStack.top();
    forExpressionBlockStack.pop();
    BasicBlock* forAfterBlock = forAfterBlockStack.top();
    forAfterBlockStack.pop();
    BasicBlock* currentBlock = builder->GetInsertBlock();
    if (currentBlock->getTerminator() == nullptr) {
        builder->CreateBr(forExpressionBlock);
    }
    builder->SetInsertPoint(forAfterBlock);
}