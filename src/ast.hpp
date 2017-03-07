#ifndef PPAP_AST_H
#define PPAP_AST_H

#include <string>
#include <vector>

namespace PPAP
{
	namespace AST_TYPE
	{
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
	}
	
	class AST
	{
	public:
		virtual ~AST() { }
		const AST_TYPE::AST_TYPE type;
		uint32_t lineno, col_offset;
		AST(AST_TYPE::AST_TYPE type, uint32_t lineno = 0, uint32_t col_offset = 0): type(type), lineno(lineno), col_offset(col_offset) { }
	};

	class AST_expr: public AST
	{
	public:
		AST_expr(AST_TYPE::AST_TYPE type, uint32_t lineno = 0, uint32_t col_offset = 0): AST(type, lineno, col_offset) { }
	};

	class AST_stmt: public AST
	{
	public:
		AST_stmt(AST_TYPE::AST_TYPE type): AST(type) { }
	};

	class AST_Expr: public AST_stmt
	{
	public:
		AST_expr* value;
		AST_Expr(): AST_stmt(AST_TYPE::Expr) { }
		AST_Expr(AST_expr *value): AST_stmt(AST_TYPE::Expr), value(value) { } 
	};

	class AST_Assign: public AST_stmt
	{
	public:
		std::vector<AST_expr*> targets;
		AST_expr* value;
		AST_Assign(): AST_stmt(AST_TYPE::Assign) { }
	};

	class AST_BinOp: public AST_expr
	{
	public:
		AST_TYPE::AST_TYPE op_type;
		AST_expr *left, *right;
		AST_BinOp(): AST_expr(AST_TYPE::BinOp) { }
	};

	class AST_Num: public AST_expr
	{
	public:
		enum NumType: unsigned char
		{
			FLOAT = 0x20,
		} num_type;
		union
		{
			double n_float;
		};
		AST_Num(): AST_expr(AST_TYPE::Num) { }
	};

	class AST_Module: public AST
	{
	public:
		std::vector<AST_stmt *> body;
		AST_Module(): AST(AST_TYPE::Module) { }
	};
}
#endif