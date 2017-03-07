#ifndef PPAP_CONVERT_HPP
#define PPAP_CONVERT_HPP

#include <string>
#include "math.h"
#include "ast.hpp"
#include "Python.h"
#include "cpython-ast.h"

namespace PPAP
{
	AST *CpythonToPPAP(mod_ty mod, std::string fn);
}

#endif
