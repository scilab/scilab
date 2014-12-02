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
/*--------------------------------------------------------------------------*/
ELEMENTARY_FUNCTIONS_IMPEXP int gw_elementary_functions(void);
/*--------------------------------------------------------------------------*/
int sci_abs(char *fname, unsigned long fname_len);
int sci_real(char *fname, unsigned long fname_len);
int sci_imag(char *fname, unsigned long fname_len);
int sci_conj(char *fname, unsigned long fname_len);
int sci_round(char *fname, unsigned long fname_len);
int sci_int(char *fname, unsigned long fname_len);
int sci_size(char *fname, unsigned long fname_len);
int sci_sum(char *fname, unsigned long fname_len);
int sci_prod(char *fname, unsigned long fname_len);
int sci_diag(char *fname, unsigned long fname_len);
int sci_triu(char *fname, unsigned long fname_len);
int sci_tril(char *fname, unsigned long fname_len);
int sci_eye(char *fname, unsigned long fname_len);
int sci_rand(char *fname, unsigned long fname_len);
int sci_ones(char *fname, unsigned long fname_len);
int sci_maxi(char *fname, unsigned long fname_len);
int sci_kron(char *fname, unsigned long fname_len);
int sci_scimatrix(char *fname, unsigned long fname_len);
int sci_sin(char *fname, unsigned long fname_len);
int sci_cos(char *fname, unsigned long fname_len);
int sci_atan(char *fname, unsigned long fname_len);
int sci_exp(char *fname, unsigned long fname_len);
int sci_sqrt(char *fname, unsigned long fname_len);
int sci_log(char *fname, unsigned long fname_len);
int sci_chinesehat(char *fname, unsigned long fname_len);
int sci_sign(char *fname, unsigned long fname_len);
int sci_clean(char *fname, unsigned long fname_len);
int sci_floor(char *fname, unsigned long fname_len);
int sci_ceil(char *fname, unsigned long fname_len);
int sci_expm(char *fname, unsigned long fname_len);
int sci_cumsum(char *fname, unsigned long fname_len);
int sci_cumprod(char *fname, unsigned long fname_len);
int sci_testmatrix(char *fname, unsigned long fname_len);
int sci_isreal(char *fname, unsigned long fname_len);
int sci_frexp(char *fname, unsigned long fname_len);
int sci_zeros(char *fname, unsigned long fname_len);
int sci_tan(char *fname, unsigned long fname_len);
int sci_log1p(char *fname, unsigned long fname_len);
int sci_imult(char *fname, unsigned long fname_len);
int sci_asin(char *fname, unsigned long fname_len);
int sci_acos(char *fname, unsigned long fname_len);
int sci_number_properties(char *fname, unsigned long fname_len);
int sci_nearfloat(char *fname, unsigned long fname_len);
int sci_dsearch(char *fname, unsigned long fname_len);
int sci_isequal(char *fname, unsigned long fname_len);
int sci_spones(char *fname, unsigned long fname_len);
int sci_gsort(char *fname, unsigned long fname_len);
int sci_isequalbitwise(char *fname, unsigned long fname_len);
int sci_rat(char *fname, unsigned long fname_len);
int sci_base2dec(char *fname, unsigned long fname_len);
int sci_dec2base(char *fname, unsigned long fname_len);
int C2F(sci_find)(char *fname, unsigned long fname_len);
int sci_log10(char *fname, unsigned long fname_len);
#endif /*  __GW_ELEMENTARIES_FUNCTIONS__ */
/*--------------------------------------------------------------------------*/

