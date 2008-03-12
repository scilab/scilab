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
int C2F(sci_abs) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_real) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_imag) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_conj) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_round) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_int) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_size) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_sum) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_prod) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_diag) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_triu) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_tril) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_eye) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_rand) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_ones) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_maxi) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_kron) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_scimatrix) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_sin) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_cos) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_atan) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_exp) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_sqrt) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_log) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_chinesehat) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_sign) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_clean) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_floor) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_ceil) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_expm) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_cumsum) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_cumprod) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_testmatrix) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_isreal) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_frexp) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_zeros) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_tan) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_log1p) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_imult) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_asin) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_acos) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_number_properties) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_nearfloat) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_dsearch) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_isequal) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_spones) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_gsort) _PARAMS((char *fname, unsigned long fname_len));
int C2F(sci_isequalbitwise) _PARAMS((char *fname, unsigned long fname_len));
#endif /*  __GW_ELEMENTARIES_FUNCTIONS__ */
/*--------------------------------------------------------------------------*/

