
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET - <allan.cornet@inria.fr>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GW_LINEAR_ALGEBRA__
#define __GW_LINEAR_ALGEBRA__
#include "machine.h"
#include "api_common.h"
/*--------------------------------------------------------------------------*/
#define SCI_DOUBLE 1
#define STRING  10
#define REAL 0
#define COMPLEX 1
#define FUNCTION 13
#define STRINGREAL 27
#define STRINGCOMPLEX 12
#define YES 1
#define NO 0
/*--------------------------------------------------------------------------*/
int gw_linear_algebra(void);
/*--------------------------------------------------------------------------*/
int C2F(inthess)(char *fname,unsigned long fname_len);
int C2F(intqr)(char *fname,unsigned long fname_len);
int C2F(intbackslash)(char *fname,unsigned long fname_len);
int C2F(intbdiagr)(char *fname, long unsigned int fname_len);
int C2F(intdet)(char *fname,unsigned long fname_len);
int C2F(intinv)(char *fname,unsigned long fname_len);
int C2F(intlu)(char *fname,unsigned long fname_len);
int C2F(intrcond)(char *fname,unsigned long fname_len);
int C2F(intslash)(char *fname,unsigned long fname_len);
int C2F(intsvd)(char *fname,unsigned long fname_len);
int C2F(intlsq)(char *fname,unsigned long fname_len);
int C2F(intchol)(char *fname,unsigned long fname_len);
int C2F(inteig) (char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
/* used in 'eig' */
int sci_dsyev(char *fname, unsigned long fname_len);
int sci_dgeev(char *fname, unsigned long fname_len);
int sci_zgeev(char *fname, unsigned long fname_len);
int sci_zheev(char *fname, unsigned long fname_len);
int sci_dggev(char *fname, unsigned long fname_len);
int sci_zggev(char *fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
int C2F(intbalanc)(char *fname,unsigned long fname_len);
int C2F(intschur)(char *fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_LINEAR_ALGEBRA__ */
/*--------------------------------------------------------------------------*/

