#include "./llvm_controller.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"

using namespace llvm;

LLVMController::LLVMController() {
    context = new LLVMContext();
    module = new Module("C-- Compiler", *context);
    builder = new IRBuilder<>(*context);
}
