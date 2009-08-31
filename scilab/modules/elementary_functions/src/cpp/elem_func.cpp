/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "elem_func.hxx"
extern "C"
{
	#include "gw_elementary_functions.h"
}

using namespace types;

bool ElemFuncModule::Load()
{
	symbol::Context::getInstance()->AddFunction(Function::createFunction("abs", &sci_abs, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("acos", &sci_acos, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("asin", &sci_asin, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("atan", &sci_atan, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("ceil", &sci_ceil, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("clean", &sci_clean, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("conj", &sci_conj, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("cos", &sci_cos, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("cumprod", &sci_cumprod, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("cumsum", &sci_cumsum, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("diag", &sci_diag, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("dsearch", &sci_dsearch, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("exp", &sci_exp, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("expm", &sci_expm, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("eye", &sci_eye, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("floor", &sci_floor, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("frexp", &sci_frexp, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("gsort", &sci_gsort, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("imag", &sci_imag, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("imult", &sci_imult, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("int", &sci_int, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("isreal", &sci_isreal, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("kron", &sci_kron, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("log", &sci_log, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("log1p", &sci_log1p, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("matrix", &sci_scimatrix, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("maxi", &sci_maxi, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("mini", &sci_mini, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("nearfloat", &sci_nearfloat, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("number_properties", &sci_number_properties, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("ones", &sci_ones, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("prod", &sci_prod, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("real", &sci_real, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("round", &sci_round, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("sign", &sci_sign, "elementary_function"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("sin", &sci_sin, "elementary_function"));
	return true;
}

