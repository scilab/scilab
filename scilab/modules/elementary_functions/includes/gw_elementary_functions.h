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
#include "api_common.h"
/*--------------------------------------------------------------------------*/
int gw_elementary_functions(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_abs)(char *fname,unsigned long fname_len);
int C2F(sci_real)(char *fname,unsigned long fname_len);
int C2F(sci_imag)(char *fname,unsigned long fname_len);
int C2F(sci_conj)(char *fname,unsigned long fname_len);
int C2F(sci_round)(char *fname,unsigned long fname_len);
int C2F(sci_int)(char *fname,unsigned long fname_len);
int C2F(sci_size)(char *fname,unsigned long fname_len);
int C2F(sci_sum)(char *fname,unsigned long fname_len);
int C2F(sci_prod)(char *fname,unsigned long fname_len);
int C2F(sci_diag)(char *fname,unsigned long fname_len);
int C2F(sci_triu)(char *fname,unsigned long fname_len);
int C2F(sci_tril)(char *fname,unsigned long fname_len);
int C2F(sci_eye)(char *fname,unsigned long fname_len);
int C2F(sci_rand)(char *fname,unsigned long fname_len);
int C2F(sci_ones)(char *fname,unsigned long fname_len);
int C2F(sci_maxi)(char *fname,unsigned long fname_len);
int C2F(sci_kron)(char *fname,unsigned long fname_len);
int C2F(sci_scimatrix)(char *fname,unsigned long fname_len);
int C2F(sci_sin)(char *fname,unsigned long fname_len);
int C2F(sci_cos)(char *fname,unsigned long fname_len);
int C2F(sci_atan)(char *fname,unsigned long fname_len);
int C2F(sci_exp)(char *fname,unsigned long fname_len);
int C2F(sci_sqrt)(char *fname,unsigned long fname_len);
int C2F(sci_log)(char *fname,unsigned long fname_len);
int C2F(sci_chinesehat)(char *fname,unsigned long fname_len);
int C2F(sci_sign)(char *fname,unsigned long fname_len);
int C2F(sci_clean)(char *fname,unsigned long fname_len);
int C2F(sci_floor)(char *fname,unsigned long fname_len);
int C2F(sci_ceil)(char *fname,unsigned long fname_len);
int C2F(sci_expm)(char *fname,unsigned long fname_len);
int C2F(sci_cumsum)(char *fname,unsigned long fname_len);
int C2F(sci_cumprod)(char *fname,unsigned long fname_len);
int C2F(sci_testmatrix)(char *fname,unsigned long fname_len);
int C2F(sci_isreal)(char *fname,unsigned long fname_len);
int C2F(sci_frexp)(char *fname,unsigned long fname_len);
int C2F(sci_zeros)(char *fname,unsigned long fname_len);
int C2F(sci_tan)(char *fname,unsigned long fname_len);
int C2F(sci_log1p)(char *fname,unsigned long fname_len);
int C2F(sci_imult)(char *fname,unsigned long fname_len);
int C2F(sci_asin)(char *fname,unsigned long fname_len);
int C2F(sci_acos)(char *fname,unsigned long fname_len);
int C2F(sci_number_properties)(char *fname,unsigned long fname_len);
int C2F(sci_nearfloat)(char *fname,unsigned long fname_len);
int C2F(sci_dsearch)(char *fname,unsigned long fname_len);
int C2F(sci_isequal)(char *fname,unsigned long fname_len);
int C2F(sci_spones)(char *fname,unsigned long fname_len);
int C2F(sci_gsort)(char *fname, unsigned long fname_len);
int C2F(sci_isequalbitwise)(char *fname, unsigned long fname_len);
int C2F(sci_sort)(char *fname, unsigned long fname_len);
#endif /*  __GW_ELEMENTARIES_FUNCTIONS__ */
/*--------------------------------------------------------------------------*/

