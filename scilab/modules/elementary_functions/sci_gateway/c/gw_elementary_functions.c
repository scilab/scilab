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
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
	{C2F(sci_abs),"abs"},
	{C2F(sci_real),"real"},
	{C2F(sci_imag),"imag"},
	{C2F(sci_conj),"conj"},
	{C2F(sci_round),"round"},
	{C2F(sci_int),"int"},
	{C2F(sci_size),"size"},
	{C2F(sci_sum),"sum"},
	{C2F(sci_prod),"prod"},
	{C2F(sci_diag),"diag"},
	{C2F(sci_triu),"triu"},
	{C2F(sci_tril),"tril"},
	{C2F(sci_eye),"eye"},
	{C2F(sci_rand),"rand"},
	{C2F(sci_ones),"ones"},
	{C2F(sci_maxi),"maxi"},
	{C2F(sci_maxi),"mini"},
	{C2F(sci_kron),"kron"},
	{C2F(sci_kron),"kron"},
	{C2F(sci_kron),"kron"},
	{C2F(sci_scimatrix),"matrix"},	
	{C2F(sci_sin),"sin"},
	{C2F(sci_cos),"cos"},
	{C2F(sci_atan),"atan"},
	{C2F(sci_exp),"exp"},
	{C2F(sci_sqrt),"sqrt"},
	{C2F(sci_log),"log"},
	{C2F(sci_chinesehat),"^"},
	{C2F(sci_sign),"sign"},
	{C2F(sci_clean),"clean"},
	{C2F(sci_floor),"floor"},
	{C2F(sci_ceil),"ceil"},
	{C2F(sci_expm),"expm"},
	{C2F(sci_cumsum),"cumsum"},
	{C2F(sci_cumprod),"cumprod"},
	{C2F(sci_testmatrix),"testmatrix"},
	{C2F(sci_isreal),"isreal"},
	{C2F(sci_frexp),"frexp"},
	{C2F(sci_zeros),"zeros"},
	{C2F(sci_tan),"tan"},
	{C2F(sci_log1p),"log1p"},
	{C2F(sci_imult),"imult"},
	{C2F(sci_asin),"asin"},
	{C2F(sci_acos),"acos"},
	{C2F(sci_number_properties),"number_properties"},
	{C2F(sci_nearfloat),"nearfloat"},
	{C2F(sci_dsearch),"dsearch"},
	{C2F(sci_isequal),"isequal"},
	{C2F(sci_spones),"spones"},
	{C2F(sci_gsort),"gsort"},
	{C2F(sci_isequalbitwise),"isequalbitwise"},
	{C2F(sci_sort),"sort"}
};
/*--------------------------------------------------------------------------*/
int gw_elementary_functions(void)
{  
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/
