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
//C_GATEWAY_PROTOTYPE(sci_abs);
//C_GATEWAY_PROTOTYPE(sci_acos);
//C_GATEWAY_PROTOTYPE(sci_asin);
//C_GATEWAY_PROTOTYPE(sci_atan);
//C_GATEWAY_PROTOTYPE(sci_ceil);
//C_GATEWAY_PROTOTYPE(sci_clean);
//C_GATEWAY_PROTOTYPE(sci_conj);
//C_GATEWAY_PROTOTYPE(sci_cos);
//C_GATEWAY_PROTOTYPE(sci_cumprod);
//C_GATEWAY_PROTOTYPE(sci_cumsum);
//C_GATEWAY_PROTOTYPE(sci_diag);
//C_GATEWAY_PROTOTYPE(sci_dsearch);
//C_GATEWAY_PROTOTYPE(sci_exp);
//C_GATEWAY_PROTOTYPE(sci_expm);
//C_GATEWAY_PROTOTYPE(sci_floor);
//C_GATEWAY_PROTOTYPE(sci_frexp);
//C_GATEWAY_PROTOTYPE(sci_gsort);
//C_GATEWAY_PROTOTYPE(sci_imag);
//C_GATEWAY_PROTOTYPE(sci_imult);
//C_GATEWAY_PROTOTYPE(sci_int);
//C_GATEWAY_PROTOTYPE(sci_isreal);
//C_GATEWAY_PROTOTYPE(sci_kron);
//C_GATEWAY_PROTOTYPE(sci_log);
//C_GATEWAY_PROTOTYPE(sci_log1p);
//C_GATEWAY_PROTOTYPE(sci_scimatrix);
//C_GATEWAY_PROTOTYPE(sci_maxi);
//C_GATEWAY_PROTOTYPE(sci_mini);
C_GATEWAY_PROTOTYPE(sci_nearfloat);
C_GATEWAY_PROTOTYPE(sci_number_properties);
//C_GATEWAY_PROTOTYPE(sci_prod);
//C_GATEWAY_PROTOTYPE(sci_real);
//C_GATEWAY_PROTOTYPE(sci_round);
//C_GATEWAY_PROTOTYPE(sci_sign);
//C_GATEWAY_PROTOTYPE(sci_sin);
//C_GATEWAY_PROTOTYPE(sci_sqrt);
//C_GATEWAY_PROTOTYPE(sci_tan);
C_GATEWAY_PROTOTYPE(sci_testmatrix);
//C_GATEWAY_PROTOTYPE(sci_tril);
//C_GATEWAY_PROTOTYPE(sci_triu);
C_GATEWAY_PROTOTYPE(sci_base2dec);
C_GATEWAY_PROTOTYPE(sci_dec2base);
C_GATEWAY_PROTOTYPE(sci_log10);

#endif /*  __GW_ELEMENTARIES_FUNCTIONS__ */
/*--------------------------------------------------------------------------*/

