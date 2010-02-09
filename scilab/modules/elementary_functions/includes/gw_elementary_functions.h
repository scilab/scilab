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
/*--------------------------------------------------------------------------*/
int gw_elementary_functions(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_chinesehat)(char *fname,unsigned long fname_len);
int C2F(sci_spones)(char *fname,unsigned long fname_len);
int C2F(sci_isequalbitwise)(char *fname, unsigned long fname_len);

// YaSp
int sci_abs(char *fname, int* _piKey);
int sci_acos(char *fname, int* _piKey);
int sci_asin(char *fname, int* _piKey);
int sci_atan(char *fname, int* _piKey);
int sci_ceil(char *fname, int* _piKey);
int sci_clean(char *fname, int* _piKey);
int sci_conj(char *fname, int* _piKey);
int sci_cos(char *fname, int* _piKey);
int sci_cumprod(char *fname, int* _piKey);
int sci_cumsum(char *fname, int* _piKey);
int sci_diag(char *fname, int* _piKey);
int sci_dsearch(char *fname, int* _piKey);
int sci_exp(char *fname, int* _piKey);
int sci_expm(char *fname, int* _piKey);
int sci_eye(char *fname, int* _piKey);
int sci_floor(char *fname, int* _piKey);
int sci_frexp(char *fname, int* _piKey);
int sci_gsort(char *fname, int* _piKey);
int sci_imag(char *fname, int* _piKey);
int sci_imult(char *fname, int* _piKey);
int sci_int(char *fname, int* _piKey);
//int sci_isequal(char *fname, int* _piKey); C++ interface
int sci_isreal(char *fname, int* _piKey);
int sci_kron(char *fname, int* _piKey);
int sci_log(char *fname, int* _piKey);
int sci_log1p(char *fname, int* _piKey);
int sci_scimatrix(char *fname, int* _piKey);
int sci_maxi(char *fname, int* _piKey);
int sci_mini(char *fname, int* _piKey);
int sci_nearfloat(char *fname, int* _piKey);
int sci_number_properties(char *fname, int* _piKey);
int sci_ones(char *fname, int* _piKey);
int sci_prod(char *fname, int* _piKey);
int sci_rand(char *fname, int* _piKey);
int sci_real(char *fname, int* _piKey);
int sci_round(char *fname, int* _piKey);
int sci_sign(char *fname, int* _piKey);
int sci_sin(char *fname, int* _piKey);
int sci_size(char *fnam, int* _piKey);
int sci_sqrt(char *fname, int* _piKey);
int sci_sum(char *fname, int* _piKey);
int sci_tan(char *fname, int* _piKey);
int sci_testmatrix(char *fname, int* _piKey);
int sci_tril(char *fname, int* _piKey);
int sci_triu(char *fname, int* _piKey);
int sci_zeros(char *fname, int* _piKey);

#endif /*  __GW_ELEMENTARIES_FUNCTIONS__ */
/*--------------------------------------------------------------------------*/

