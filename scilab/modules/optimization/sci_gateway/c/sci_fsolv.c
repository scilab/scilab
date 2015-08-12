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
#include "gw_optimization.h"
#include "machine.h"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
/* bug 3161 (F2C)*/
/* common need to be defined and exported from C */
__declspec (dllexport) struct
{
    char namef[25], namej[25];
} C2F(csolve);

__declspec (dllexport) struct
{
    double t0, tf, dti, dtf, ermx;
    int iu[5], nuc, nuv, ilin, nti, ntf, ny, nea, itmx, nex, nob, ntob,
        ntobi, nitu, ndtu;
} C2F(icsez);

#endif
/*--------------------------------------------------------------------------*/
extern int C2F(scisolv)(); /* FORTRAN subroutine */
/*--------------------------------------------------------------------------*/
int sci_fsolve(char *fname, unsigned long fname_len)
{
    C2F(scisolv)(fname, fname_len);
    return 0;
}
/*--------------------------------------------------------------------------*/
