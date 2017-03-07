#include "convert.hpp"
#include "math.h"
#include "Python.h"
#include "ast.hpp"
#include "cpython-ast.h"
#include <string>
#include <vector>

namespace PPAP
{

	class Converter
	{
	private:
		std::string fn;
	public:
		Converter(std::string &fn): fn(fn) { }

		template <typename T, typename P> std::vector<P> convert(asdl_seq* seq)
		{
			std::vector<P> rtn;
			if (!seq)
				return rtn;
			for (int i = 0; i < seq->size; i++)	rtn.push_back(convert((T)seq->elements[i]));
			return rtn;
 	   }

		template <typename T, typename P> std::vector<P> convert(asdl_int_seq* seq)
		{
			std::vector<P> rtn;
			if (!seq) return rtn;
			for (int i = 0; i < seq->size; i++) rtn.push_back(convert((T)seq->elements[i]));
			return rtn;
		}

		AST_expr* _convert(expr_ty expr)
		{
			switch (expr->kind)
			{
			case BinOp_kind:
			{
				auto r = new AST_BinOp();
				auto v = expr->v.BinOp;
				r->left = convert(v.left);
				r->op_type = convert(v.op);
				r->right = convert(v.right);
				return r;
			}
			case Num_kind:
			{
				PyObject *o = expr->v.Num.n;
				if (o->ob_type == &PyFloat_Type)
				{
					auto r = new AST_Num();
					r->num_type = AST_Num::FLOAT;
					r->n_float = PyFloat_AsDouble(o);
					return r;
				}
			}
			}
		}

		AST_expr *convert(expr_ty expr)
		{
			if (!expr) return NULL;
			auto r = _convert(expr);
			r->lineno = expr->lineno;
			r->col_offset = expr->col_offset;
			return r;
		}

		AST_stmt *_convert(stmt_ty stmt)
		{
			switch (stmt->kind)
			{
			case Assign_kind:
			{
				auto r = new AST_Assign();
				auto v = stmt->v.Assign;
				r->targets = convert<expr_ty, AST_expr*>(v.targets);
				r->value = convert(v.value);
				return r;
			}
			case Expr_kind:
			{
				auto r = new AST_Expr();
				auto v = stmt->v.Expr;
				r->value = convert(v.value);
				return r;
			}
			}
			return nullptr;
		}

		AST_stmt *convert(stmt_ty stmt)
		{
			auto r = _convert(stmt);
			r->lineno = stmt->lineno;
			r->col_offset = stmt->col_offset;
			if (stmt->lineno == 0) r->lineno = 1;
			return r;
		}

		#define CASE(N) case N: return AST_TYPE::N

		AST_TYPE::AST_TYPE convert(operator_ty op)
		{
			switch (op)
			{
				CASE(Add);
			}
		}

		template <typename T, typename P> void convertAll(asdl_seq* seq, std::vector<P>& vec) { vec = convert<T, P>(seq); }

		AST *convert(mod_ty mod)
		{
			if (mod->kind == Module_kind)
			{
				AST_Module *rtn = new AST_Module();
				rtn->lineno = 1;
				convertAll<stmt_ty>(mod->v.Module.body, rtn->body);
				return rtn;
			}
			else return nullptr;
		}
	};
	AST *CpythonToPPAP(mod_ty mod, std::string fn)
	{
		Converter c(fn);
		return c.convert(mod);
	}

}