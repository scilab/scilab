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

#define MODULE_NAME "elementary_functions"
extern "C"
{
	#include "gw_elementary_functions.h"
}

using namespace types;

bool ElemFuncModule::Load()
{
	symbol::Context::getInstance()->AddFunction(Function::createFunction("abs", &sci_abs, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("acos", &sci_acos, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("asin", &sci_asin, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("atan", &sci_atan, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("ceil", &sci_ceil, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("clean", &sci_clean, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("conj", &sci_conj, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("cos", &sci_cos, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("cumprod", &sci_cumprod, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("cumsum", &sci_cumsum, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("diag", &sci_diag, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("dsearch", &sci_dsearch, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("exp", &sci_exp, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("expm", &sci_expm, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("eye", &sci_eye, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("floor", &sci_floor, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("frexp", &sci_frexp, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("gsort", &sci_gsort, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("imag", &sci_imag, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("imult", &sci_imult, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("int", &sci_int, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("isreal", &sci_isreal, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("isequal", &sci_isequal, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("kron", &sci_kron, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("log", &sci_log, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("log1p", &sci_log1p, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("matrix", &sci_scimatrix, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("maxi", &sci_maxi, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("mini", &sci_mini, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("nearfloat", &sci_nearfloat, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("number_properties", &sci_number_properties, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("ones", &sci_ones, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("prod", &sci_prod, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("rand", &sci_rand, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("real", &sci_real, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("round", &sci_round, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("sign", &sci_sign, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("sin", &sci_sin, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("size", &sci_size, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("sum", &sci_sum, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("tan", &sci_tan, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("testmatrix", &sci_testmatrix, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("tril", &sci_tril, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("zeros", &sci_zeros, MODULE_NAME));

	return true;
}

