#include "ast.hpp"

using namespace llvm;
using namespace PPAP;

LLVMContext TheContext;
IRBuilder<> Builder(TheContext);

Value *AST_BinOp::codegen() {
    Value *L = left->codegen();
    Value *R = right->codegen();
    switch (op) {
        case AST_TYPE::Add:
            return Builder.CreateFAdd(L, R, "addtmp");
    }
}

Value *AST_Float::codegen() {
    return ConstantFP::get(TheContext, APFloat(this->n));
}
