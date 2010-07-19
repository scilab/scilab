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
/*--------------------------------------------------------------------------*/
ELEMENTARY_FUNCTIONS_IMPEXP int gw_elementary_functions(void);
/*--------------------------------------------------------------------------*/
int sci_chinesehat(char *fname,unsigned long fname_len);
int sci_spones(char *fname,unsigned long fname_len);
int sci_isequalbitwise(char *fname, unsigned long fname_len);

// YaSp
ELEMENTARY_FUNCTIONS_IMPEXP int sci_abs(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_acos(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_asin(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_atan(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_ceil(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_clean(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_conj(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_cos(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_cumprod(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_cumsum(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_diag(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_dsearch(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_exp(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_expm(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_eye(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_floor(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_frexp(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_gsort(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_imag(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_imult(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_int(char *fname, int* _piKey);
//ELEMENTARY_FUNCTIONS_IMPEXP int sci_isequal(char *fname, int* _piKey); C++ interface
ELEMENTARY_FUNCTIONS_IMPEXP int sci_isreal(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_kron(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_log(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_log1p(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_scimatrix(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_maxi(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_mini(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_nearfloat(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_number_properties(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_ones(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_prod(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_rand(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_real(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_round(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_sign(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_sin(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_sqrt(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_sum(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_tan(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_testmatrix(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_tril(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_triu(char *fname, int* _piKey);
ELEMENTARY_FUNCTIONS_IMPEXP int sci_zeros(char *fname, int* _piKey);

#endif /*  __GW_ELEMENTARIES_FUNCTIONS__ */
/*--------------------------------------------------------------------------*/

