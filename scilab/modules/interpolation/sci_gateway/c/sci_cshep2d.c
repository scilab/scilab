/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Bruno Pincon
 * Copyright (C) DIGITEO - 2012 - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*/
#include <string.h>
#include "gw_interpolation.h"
#include "api_scilab.h"
#include "stack-c.h"
#include "interpolation.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
extern int C2F(cshep2) (int *n, double *x, double *y, double *z, int *nc, int *nw,
		 int *nr, int *lcell, int *lnext, double *xmin, double *xmax,
		 double *dx, double *dy, double *rmax, double *rw, double *a, int *ier);
/*--------------------------------------------------------------------------*/
int intcshep2d(char *fname,unsigned long fname_len)
{
    static char *Str[] = {"cshep2d", "xyz", "lcell", "lnext", "grdim", "rmax", "rw", "a"};
    int minrhs = 1, maxrhs = 1, minlhs = 1, maxlhs = 1;
    int n = 0, dim = 0, nc = 0, nw = 0, nr = 0, one = 1, four = 4, eight = 8, nine = 9, ier = 0;
    int lxyz = 0, lxyzn = 0, lcell = 0, lnext = 0, lgrid = 0, lrmax = 0, lrw = 0, la = 0, ltlist = 0, lar = 0;

    double *xyz = NULL, *grid = NULL;

    CheckRhs(minrhs, maxrhs);
    CheckLhs(minlhs, maxlhs);

    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &n, &dim, &lxyz);
    if ((dim != 3)  ||  (n < 10))
    {
        Scierror(999,_("%s: xyz must be a (n,3) real matrix with n >= 10\n"), fname);
        return 0;
    }
    else
    {
        SciErr sciErr;
        int *piAddressVarOne = NULL;
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if (isVarComplex(pvApiCtx, piAddressVarOne))
        {
            Scierror(202, _("%s: Wrong type for argument %d: Real matrix expected.\n"), fname, 1);
            return 0;
        }
    }

    /* choix pour nc (peut etre futur parametre optionnel) */
    nc = Min(17, n - 1);
    /* choix pour nw */
    nw = Min(30, n - 1);
    /* choix pour nr (grille nr x nr) */
    nr = (int) sqrt( n/3.0 ); /* comme n >= 10 nr >= 1 */

    /* all the information for the "interpolant" will be stored
    * in a tlist (which also contains the entry xyz)
    */
    CreateVar(2, TYPED_LIST_DATATYPE, &eight, &one, &ltlist);
    CreateListVarFromPtr(2, 1, MATRIX_OF_STRING_DATATYPE, &one,  &eight, Str);
    CreateListVarFrom(2, 2, MATRIX_OF_DOUBLE_DATATYPE, &n ,   &dim,  &lxyzn, &lxyz);
    lcell = 4; lar = -1;
    CreateListVarFrom(2, 3,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &nr,   &nr,   &lcell, &lar); /* lcell */
    lnext = 4; lar = -1;
    CreateListVarFrom(2, 4,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &one,  &n,    &lnext, &lar); /* lnext */
    lar = -1;
    CreateListVarFrom(2, 5,MATRIX_OF_DOUBLE_DATATYPE, &one,  &four, &lgrid, &lar); /* xmin, ymin, dx, dy */
    lar = -1;
    CreateListVarFrom(2, 6,MATRIX_OF_DOUBLE_DATATYPE, &one,  &one,  &lrmax, &lar); /* rmax */
    lar = -1;
    CreateListVarFrom(2, 7,MATRIX_OF_DOUBLE_DATATYPE, &one,  &n,    &lrw,   &lar); /* rw */
    lar = -1;
    CreateListVarFrom(2, 8,MATRIX_OF_DOUBLE_DATATYPE, &nine, &n,    &la,    &lar); /* a */
    grid = stk(lgrid);
    xyz = stk(lxyz);

    /*      SUBROUTINE CSHEP2 (N,X,Y,F,NC,NW,NR, LCELL,LNEXT,XMIN,
    *                         YMIN,DX,DY,RMAX,RW,A,IER)
    */
    C2F(cshep2) ( &n, xyz, &xyz[n], &xyz[2*n], &nc, &nw, &nr, istk(lcell),
        istk(lnext), grid, &grid[1], &grid[2], &grid[3], stk(lrmax),
        stk(lrw), stk(la), &ier);

    if ( ier != 0 )
    {
        Scierror(999,_("%s: Duplicate nodes or all nodes colinears (ier = %d).\n"), fname, ier);
        return 0;
    }

    LhsVar(1) = 2;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
