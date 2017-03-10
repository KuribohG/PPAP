#ifndef PPAP_AST_H
#define PPAP_AST_H

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

#include <string>
#include <vector>
#include <iostream>

using namespace llvm;

namespace PPAP
{
	namespace AST_TYPE
	{
        enum Expr_context_type { Load=1, Store=2, Del=3, AugLoad=4, AugStore=5,
            Param=6 };

        enum Boolop_type { And=1, Or=2 };

        enum Operator_type { Add=1, Sub=2, Mult=3, MatMult=4, Div=5, Mod=6, Pow=7,
            LShift=8, RShift=9, BitOr=10, BitXor=11, BitAnd=12,
            FloorDiv=13 };

        enum Unaryop_type { Invert=1, Not=2, UAdd=3, USub=4 };

        enum Cmpop_type { Eq=1, NotEq=2, Lt=3, LtE=4, Gt=5, GtE=6, Is=7, IsNot=8,
            In=9, NotIn=10 };

        enum Mod_type {Module=1, Interactive=2, Expression=3,
            Suite=4};

        enum Stmt_type {FunctionDef=1, AsyncFunctionDef=2, ClassDef=3,
            Return=4, Delete=5, Assign=6,
            AugAssign=7, AnnAssign=8, For=9,
            AsyncFor=10, While=11, If=12, With=13,
            AsyncWith=14, Raise=15, Try=16,
            Assert=17, Import=18, ImportFrom=19,
            Global=20, Nonlocal=21, Expr=22, Pass=23,
            Break=24, Continue=25};

        enum Expr_type {BoolOp=1, BinOp=2, UnaryOp=3, Lambda=4,
            IfExp=5, Dict=6, Set=7, ListComp=8,
            SetComp=9, DictComp=10, GeneratorExp=11,
            Await=12, Yield=13, YieldFrom=14,
            Compare=15, Call=16, Num=17, Str=18,
            FormattedValue=19, JoinedStr=20, Bytes=21,
            NameConstant=22, Ellipsis=23, Constant=24,
            Attribute=25, Subscript=26, Starred=27,
            Name=28, List=29, Tuple=30};

        enum Slice_type {Slice=1, ExtSlice=2, Index=3};

        enum Excepthandler_type {ExceptHandler=1};

        enum Num_type {Int=1, Float=2, Complex=3};
        /*
        #define FOREACH_TYPE(X)\
			X(Assign, 1)\
			X(BinOp, 2)\
			X(Call, 3)\
			X(Expr, 4)\
			X(Add, 5)\
			X(Num, 6)\
			X(Module, 7)
		#define GENERATE_ENUM(ENUM, N) ENUM = N,
		#define GENERATE_STRING(STRING, N) m[N] = #STRING;

		enum AST_TYPE: unsigned char { FOREACH_TYPE(GENERATE_ENUM) };

		#undef FOREACH_TYPE
		#undef GENERATE_ENUM
		#undef GENERATE_STRING
        */
	}
	
	class AST
	{
	public:
		virtual ~AST() { }
		virtual void visit(std::ostream &o, int dep);
	};

	class AST_expr: public AST
	{
	public:
        AST_TYPE::Expr_type type;
        int lineno;
        int col_offset;
		AST_expr(AST_TYPE::Expr_type type, int lineno = 0, int col_offset = 0)
                : type(type), lineno(lineno), col_offset(col_offset) { }
        virtual Value *codegen() = 0;
		virtual void visit(std::ostream &o, int dep);
	};

	class AST_stmt: public AST
	{
	public:
        AST_TYPE::Stmt_type type;
        int lineno;
        int col_offset;
		AST_stmt(AST_TYPE::Stmt_type type, int lineno = 0, int col_offset = 0)
                : type(type), lineno(lineno), col_offset(col_offset) { }
		virtual void visit(std::ostream &o, int dep);
	};

	class AST_Expr: public AST_stmt
	{
	public:
		AST_expr* value;
		AST_Expr(): AST_stmt(AST_TYPE::Expr) { }
		AST_Expr(AST_expr *value): AST_stmt(AST_TYPE::Expr), value(value) { }
		virtual void visit(std::ostream &o, int dep);
	};

	class AST_Assign: public AST_stmt
	{
	public:
		std::vector<AST_expr*> targets;
		AST_expr* value;
		AST_Assign(): AST_stmt(AST_TYPE::Assign) { }
		virtual void visit(std::ostream &o, int dep);
	};

	class AST_BinOp: public AST_expr
	{
	public:
		AST_TYPE::Operator_type op;
		AST_expr *left, *right;
		AST_BinOp(): AST_expr(AST_TYPE::BinOp) { }
        virtual Value *codegen() override;
		virtual void visit(std::ostream &o, int dep);
	};

	class AST_Num: public AST_expr
	{
	public:
        AST_TYPE::Num_type num_type;
		AST_Num(AST_TYPE::Num_type num_type): AST_expr(AST_TYPE::Num), num_type(num_type) { }
		virtual void visit(std::ostream &o, int dep);
	};

	class AST_Name: public AST_expr
	{
	public:
		std::string id;
		AST_TYPE::Expr_context_type ctx;
		AST_Name(std::string id, AST_TYPE::Expr_context_type ctx): AST_expr(AST_TYPE::Name), id(id), ctx(ctx) { }
		virtual void visit(std::ostream &o, int dep);
	};

    class AST_Float: public AST_Num
    {
    public:
        double n;
        AST_Float(): AST_Num(AST_TYPE::Float) { }
        virtual Value *codegen() override;
		virtual void visit(std::ostream &o, int dep);
    };

    class AST_Int: public AST_Num
    {
    public:
        std::string n;
        AST_Int(): AST_Num(AST_TYPE::Int) { }
		virtual void visit(std::ostream &o, int dep);
    };

    class AST_mod: public AST
    {
    public:
        AST_TYPE::Mod_type type;
        AST_mod(AST_TYPE::Mod_type type): type(type) { }
		virtual void visit(std::ostream &o, int dep);
    };

	class AST_Module: public AST_mod
	{
	public:
		std::vector<AST_stmt *> body;
		AST_Module(): AST_mod(AST_TYPE::Module) { }
		virtual void visit(std::ostream &o, int dep);
	};
}
#endif