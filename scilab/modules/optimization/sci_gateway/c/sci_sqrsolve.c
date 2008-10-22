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
#include "gw_optim.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
/* bug 3161 (F2C)*/
/* common need to be defined and exported from C */
__declspec (dllexport) struct {
    char namef[25], namej[25];
} C2F(clsqrsolve);
#endif
/*--------------------------------------------------------------------------*/
int C2F(sci_sqrsolve)(char *fname,unsigned long fname_len)
{
	C2F(intlsqrsolve)(fname,fname_len);
	return 0;
}
/*--------------------------------------------------------------------------*/
