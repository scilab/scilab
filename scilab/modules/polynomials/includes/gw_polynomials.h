/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GW_POLYNOMIALS__
#define __GW_POLYNOMIALS__
/*--------------------------------------------------------------------------*/
#include "machine.h"
/*--------------------------------------------------------------------------*/
int gw_polynomials(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_poly)(char *fname,unsigned long fname_len);
int C2F(sci_roots)(char *fname,unsigned long fname_len);
int C2F(sci_degree)(char *fname,unsigned long fname_len);
int C2F(sci_coeff)(char *fname,unsigned long fname_len);
int C2F(sci_pppdiv)(char *fname,unsigned long fname_len);
int C2F(sci_simp)(char *fname,unsigned long fname_len);
int C2F(sci_psum)(char *fname,unsigned long fname_len);
int C2F(sci_pprod)(char *fname,unsigned long fname_len);
int C2F(sci_pdiag)(char *fname,unsigned long fname_len);
int C2F(sci_ptriu)(char *fname,unsigned long fname_len);
int C2F(sci_ptril)(char *fname,unsigned long fname_len);
int C2F(sci_bezout)(char *fname,unsigned long fname_len);
int C2F(sci_sfact)(char *fname,unsigned long fname_len);
int C2F(sci_simp_mode)(char *fname,unsigned long fname_len);
int C2F(sci_varn)(char *fname,unsigned long fname_len);
int C2F(sci_cleanp)(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_POLYNOMIALS__ */
/*--------------------------------------------------------------------------*/

