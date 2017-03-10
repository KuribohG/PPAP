#include <string>
#include <vector>
#include <iostream>
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

inline std::ostream &visit_indent(std::ostream &o, int dep) {
	while (dep--) o << "|   ";
	return o;
}

inline void AST::visit(std::ostream &o, int dep) { }

inline void AST_expr::visit(std::ostream &o, int dep) { }

inline void AST_stmt::visit(std::ostream &o, int dep) { }

inline void AST_Expr::visit(std::ostream &o, int dep) {
	visit_indent(o, dep) << "Expr:" << std::endl;
	visit_indent(o, dep) << "|-#value" << std::endl;
	value->visit(o, dep + 1);
}

inline void AST_Assign::visit(std::ostream &o, int dep) {
	visit_indent(o, dep) << "Assign:" << std::endl;
	visit_indent(o, dep) << "|-#targets" << std::endl;
	for (auto i : targets) {
		i->visit(o, dep + 1);
	}
	visit_indent(o, dep) << "|-#value" << std::endl;
	value->visit(o, dep + 1);
}

inline void AST_BinOp::visit(std::ostream &o, int dep) {
	visit_indent(o, dep) << "BinOp:" << std::endl;
	visit_indent(o, dep) << "|-#op" << std::endl;
	visit_indent(o, dep + 1) << '%' << op << std::endl;
	visit_indent(o, dep) << "|-#left" << std::endl;
	left->visit(o, dep + 1);
	visit_indent(o, dep) << "|-#right" << std::endl;
	right->visit(o, dep + 1);
}

inline void AST_Num::visit(std::ostream &o, int dep) { }

inline void AST_Name::visit(std::ostream &o, int dep) {
	visit_indent(o, dep) << "Name:" << std::endl;
	visit_indent(o, dep) << "|-#id" << std::endl;
	visit_indent(o, dep + 1) << '%' << id << std::endl;
	visit_indent(o, dep) << "|-#ctx" << std::endl;
	visit_indent(o, dep + 1) << '%' << ctx << std::endl;
}

inline void AST_Float::visit(std::ostream &o, int dep) {
	visit_indent(o, dep) << "Float:" << std::endl;
	visit_indent(o, dep) << "|-#n" << std::endl;
	visit_indent(o, dep + 1) << '%' << n << std::endl;
}

inline void AST_Int::visit(std::ostream &o, int dep) {
	visit_indent(o, dep) << "Int:" << std::endl;
	visit_indent(o, dep) << "|-#n" << std::endl;
	visit_indent(o, dep + 1) << '%' << n << std::endl;
}

inline void AST_mod::visit(std::ostream &o, int dep) { }

inline void AST_Module::visit(std::ostream &o, int dep) {
	visit_indent(o, dep) << "Module:" << std::endl;
	visit_indent(o, dep) << "|-#body" << std::endl;
	for (auto i : body) {
		i->visit(o, dep + 1);
	}
}
