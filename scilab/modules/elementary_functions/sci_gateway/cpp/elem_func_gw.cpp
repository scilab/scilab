/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "elem_func_gw.hxx"
#include "context.hxx"

#define MODULE_NAME L"elementary_functions"
extern "C"
{
	#include "gw_elementary_functions.h"
}

int ElemFuncModule::Load()
{
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"abs", &sci_abs, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"acos", &sci_acos, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"asin", &sci_asin, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"atan", &sci_atan, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"ceil", &sci_ceil, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"clean", &sci_clean, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"conj", &sci_conj, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"cos", &sci_cos, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"cumprod", &sci_cumprod, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"cumsum", &sci_cumsum, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"diag", &sci_diag, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"dsearch", &sci_dsearch, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"exp", &sci_exp, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"expm", &sci_expm, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"eye", &sci_eye, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"floor", &sci_floor, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"frexp", &sci_frexp, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"gsort", &sci_gsort, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"imag", &sci_imag, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"imult", &sci_imult, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"int", &sci_int, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"isreal", &sci_isreal, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"isequal", &sci_isequal, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"kron", &sci_kron, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"log", &sci_log, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"log1p", &sci_log1p, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"matrix", &sci_matrix, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"max", &sci_max, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"min", &sci_min, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"nearfloat", &sci_nearfloat, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"number_properties", &sci_number_properties, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"ones", &sci_ones, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"prod", &sci_prod, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"rand", &sci_rand, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"real", &sci_real, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"round", &sci_round, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"sign", &sci_sign, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"sin", &sci_sin, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"size", &sci_size, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"sqrt", &sci_sqrt, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"sum", &sci_sum, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"tan", &sci_tan, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"testmatrix", &sci_testmatrix, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"tril", &sci_tril, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"triu", &sci_triu, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"zeros", &sci_zeros, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"rat", &sci_rat, MODULE_NAME));
	return 1;
}
