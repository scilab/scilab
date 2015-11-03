/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __GW_ELEMENTARIES_FUNCTIONS__
#define __GW_ELEMENTARIES_FUNCTIONS__
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "dynlib_elementary_functions.h"
#include "c_gateway_prototype.h"
/*--------------------------------------------------------------------------*/
ELEMENTARY_FUNCTIONS_IMPEXP int gw_elementary_functions(void);
/*--------------------------------------------------------------------------*/
int sci_chinesehat(char *fname, unsigned long fname_len);
//int sci_spones(char *fname, unsigned long fname_len);
int sci_isequalbitwise(char *fname, unsigned long fname_len);

// Scilab 6
//STACK_GATEWAY_PROTOTYPE(sci_abs);
//STACK_GATEWAY_PROTOTYPE(sci_acos);
//STACK_GATEWAY_PROTOTYPE(sci_asin);
//STACK_GATEWAY_PROTOTYPE(sci_atan);
//STACK_GATEWAY_PROTOTYPE(sci_ceil);
//STACK_GATEWAY_PROTOTYPE(sci_clean);
//STACK_GATEWAY_PROTOTYPE(sci_conj);
//STACK_GATEWAY_PROTOTYPE(sci_cos);
//STACK_GATEWAY_PROTOTYPE(sci_cumprod);
//STACK_GATEWAY_PROTOTYPE(sci_cumsum);
//STACK_GATEWAY_PROTOTYPE(sci_diag);
//STACK_GATEWAY_PROTOTYPE(sci_dsearch);
//STACK_GATEWAY_PROTOTYPE(sci_exp);
//STACK_GATEWAY_PROTOTYPE(sci_expm);
//STACK_GATEWAY_PROTOTYPE(sci_floor);
//STACK_GATEWAY_PROTOTYPE(sci_frexp);
//STACK_GATEWAY_PROTOTYPE(sci_gsort);
//STACK_GATEWAY_PROTOTYPE(sci_imag);
//STACK_GATEWAY_PROTOTYPE(sci_imult);
//STACK_GATEWAY_PROTOTYPE(sci_int);
//STACK_GATEWAY_PROTOTYPE(sci_isreal);
//STACK_GATEWAY_PROTOTYPE(sci_kron);
//STACK_GATEWAY_PROTOTYPE(sci_log);
//STACK_GATEWAY_PROTOTYPE(sci_log1p);
//STACK_GATEWAY_PROTOTYPE(sci_scimatrix);
//STACK_GATEWAY_PROTOTYPE(sci_maxi);
//STACK_GATEWAY_PROTOTYPE(sci_mini);
STACK_GATEWAY_PROTOTYPE(sci_nearfloat);
STACK_GATEWAY_PROTOTYPE(sci_number_properties);
//STACK_GATEWAY_PROTOTYPE(sci_prod);
//STACK_GATEWAY_PROTOTYPE(sci_real);
//STACK_GATEWAY_PROTOTYPE(sci_round);
//STACK_GATEWAY_PROTOTYPE(sci_sign);
//STACK_GATEWAY_PROTOTYPE(sci_sin);
//STACK_GATEWAY_PROTOTYPE(sci_sqrt);
//STACK_GATEWAY_PROTOTYPE(sci_tan);
STACK_GATEWAY_PROTOTYPE(sci_testmatrix);
//STACK_GATEWAY_PROTOTYPE(sci_tril);
//STACK_GATEWAY_PROTOTYPE(sci_triu);
STACK_GATEWAY_PROTOTYPE(sci_base2dec);
STACK_GATEWAY_PROTOTYPE(sci_dec2base);
STACK_GATEWAY_PROTOTYPE(sci_log10);
C_GATEWAY_PROTOTYPE(sci_isvector);
C_GATEWAY_PROTOTYPE(sci_issquare);

#endif /*  __GW_ELEMENTARIES_FUNCTIONS__ */
/*--------------------------------------------------------------------------*/

