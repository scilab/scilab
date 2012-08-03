/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_elementary_functions.h"
#include "callFunctionFromGateway.h"
#include "recursionFunction.h"
#include "api_scilab.h"
#include "MALLOC.h"

/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
	{sci_abs, "abs"},
	{sci_real, "real"},
	{sci_imag, "imag"},
	{sci_conj, "conj"},
	{sci_round, "round"},
	{sci_int, "int"},
	{sci_size, "size"},
	{sci_sum, "sum"},
	{sci_prod, "prod"},
	{sci_diag, "diag"},
	{sci_triu, "triu"},
	{sci_tril, "tril"},
	{sci_eye, "eye"},
	{sci_rand, "rand"},
	{sci_ones, "ones"},
	{sci_maxi, "max"},
	{sci_maxi, "min"},
	{sci_kron, "kron"},
	{sci_kron, "kron"},
	{sci_kron, "kron"},
	{sci_scimatrix, "matrix"},	
	{sci_sin, "sin"},
	{sci_cos, "cos"},
	{sci_atan, "atan"},
	{sci_exp, "exp"},
	{sci_sqrt, "sqrt"},
	{sci_log, "log"},
	{sci_chinesehat, "^"},
	{sci_sign, "sign"},
	{sci_clean, "clean"},
	{sci_floor, "floor"},
	{sci_ceil, "ceil"},
	{sci_expm, "expm"},
	{sci_cumsum, "cumsum"},
	{sci_cumprod, "cumprod"},
	{sci_testmatrix, "testmatrix"},
	{sci_isreal, "isreal"},
	{sci_frexp, "frexp"},
	{sci_zeros, "zeros"},
	{sci_tan, "tan"},
	{sci_log1p, "log1p"},
	{sci_imult, "imult"},
	{sci_asin, "asin"},
	{sci_acos, "acos"},
	{sci_number_properties, "number_properties"},
	{sci_nearfloat, "nearfloat"},
	{sci_dsearch, "dsearch"},
	{sci_isequal, "isequal"},
	{sci_spones, "spones"},
	{sci_gsort, "gsort"},
	{sci_isequalbitwise, "isequalbitwise"},
    {sci_rat, "rat"},
    {sci_base2dec, "base2dec"},
    {sci_dec2base, "dec2base"}
};
/*--------------------------------------------------------------------------*/
int gw_elementary_functions(void)
{
	if(pvApiCtx == NULL)
	{
		pvApiCtx = (StrCtx*)MALLOC(sizeof(StrCtx));
	}

	pvApiCtx->pstName = (char*)Tab[Fin-1].name;
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/
