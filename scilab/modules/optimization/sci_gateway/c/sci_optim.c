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
#include "gw_optimization.h"
#include "machine.h"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
/* bug 3161 (F2C)*/
/* common need to be defined and exported from C */
__declspec (dllexport) struct {
    char nomsub[80];
} C2F(optim);

__declspec (dllexport) struct {
    int nizs, nrzs, ndzs;
} C2F(nird);

__declspec (dllexport) struct {
    double u1;
    int nc;
} C2F(fprf2c);
#endif
/*--------------------------------------------------------------------------*/
extern int C2F(scioptim)(); /* FORTRAN subroutine */
/*--------------------------------------------------------------------------*/
int sci_optim(char *fname,unsigned long fname_len)
{
	C2F(scioptim)(fname,fname_len);
	return 0;
}
/*--------------------------------------------------------------------------*/
