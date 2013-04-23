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
#include <stdlib.h>
#include "gw_elementary_functions.h"
#include "callFunctionFromGateway.h"
#include "recursionFunction.h"
#include "api_scilab.h"
#include "MALLOC.h"

/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
	{NULL, ""}, //abs
	{NULL, ""}, //real
	{NULL, ""}, //imag
	{NULL, ""}, //conj
	{NULL, ""}, //round
	{NULL, ""}, //int
	{NULL, ""}, //size
	{NULL, ""}, //sum
	{NULL, ""}, //prod
	{NULL, ""}, //diag
	{NULL, ""}, //trui
	{NULL, ""}, //tril
	{NULL, ""}, //eye
	{NULL, ""}, //rand
	{NULL ,""}, //ones
	{NULL ,""}, //max
	{NULL ,""}, //min
	{NULL ,""}, //kron
	{NULL ,""}, //kron
	{NULL ,""}, //kron
	{NULL ,""}, //kron
	{NULL ,""}, //sin
	{NULL ,""}, //cos
	{NULL ,""}, //atan
	{NULL ,""}, //exp
	{NULL ,""}, //sqrt
	{NULL ,""}, //log
	{sci_chinesehat,"^"},
	{NULL, ""}, //sign
	{NULL, ""}, //clean
	{NULL, ""}, //floor
	{NULL, ""}, //ceil
	{NULL, ""}, //expm
	{NULL, ""}, //cumsum
	{NULL, ""}, //cumprod
	{NULL, ""}, //testmatrix
	{NULL, ""}, //isreal
	{NULL, ""}, //frexp
	{NULL, ""}, //zeros
	{NULL, ""}, //tan
	{NULL, ""}, //log1p
	{NULL, ""}, //imult
	{NULL, ""}, //asin
	{NULL, ""}, //acos
	{NULL, ""}, //number_properties
	{NULL, ""}, //nearfloat
	{NULL, ""}, //dsearch
	{NULL, ""}, //isequal
	{NULL, ""}, //spones
	{NULL, ""}, //gsort
	{NULL, ""}, //isequalbitwise
    {NULL, ""}, //rat
    {NULL, ""}, //base2dec
    {NULL, ""} //dec2base
};
/*--------------------------------------------------------------------------*/
int gw_elementary_functions(void)
{
	return 0;
}
/*--------------------------------------------------------------------------*/
