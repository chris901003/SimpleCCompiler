#ifndef LLVM_CONTROLLER_H
#define LLVM_CONTROLLER_H

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"

using namespace llvm;

class LLVMController {
    private:
        LLVMContext *context;
        Module *module;
        IRBuilder<> *builder;

    public:
        LLVMController();
};

#endif