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
#ifndef __GW_ELEMENTARIES_FUNCTIONS__
#define __GW_ELEMENTARIES_FUNCTIONS__
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "dynlib_elementary_functions.h"
#include "c_gateway_prototype.h"
/*--------------------------------------------------------------------------*/
ELEMENTARY_FUNCTIONS_IMPEXP int gw_elementary_functions(void);
/*--------------------------------------------------------------------------*/
int sci_chinesehat(char *fname,unsigned long fname_len);
int sci_spones(char *fname,unsigned long fname_len);
int sci_isequalbitwise(char *fname, unsigned long fname_len);

// YaSp
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_abs);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_acos);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_asin);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_atan);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_ceil);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_clean);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_conj);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_cos);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_cumprod);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_cumsum);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_diag);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_dsearch);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_exp);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_expm);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_floor);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_frexp);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_gsort);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_imag);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_imult);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_int);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_isreal);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_kron);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_log);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_log1p);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_scimatrix);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_maxi);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_mini);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_nearfloat);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_number_properties);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_prod);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_real);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_round);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_sign);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_sin);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_sqrt);
//ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_sum);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_tan);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_testmatrix);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_tril);
ELEMENTARY_FUNCTIONS_IMPEXP C_GATEWAY_PROTOTYPE(sci_triu);

#endif /*  __GW_ELEMENTARIES_FUNCTIONS__ */
/*--------------------------------------------------------------------------*/

