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
#include "someinterp.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
#define NB_OUTMODE 6
static TableType OutModeTable[NB_OUTMODE] =
{
    { "C0"        , C0         },
    { "by_zero"   , BY_ZERO    },
    { "natural"   , NATURAL    },
    { "periodic"  , PERIODIC   },
    { "by_nan"    , BY_NAN     },
    { "linear"    , LINEAR     }
};
/*--------------------------------------------------------------------------*/
int intlinear_interpn(char *fname, unsigned long fname_len)
{
    /*  interpolation lineaire n-dimensionnelle
    *
    *   yp = linear_interpn(xp1, ..., xpn, x1, ..., xn, val, outmode)
    */
    int n = 0, mxp = 0, nxp = 0, lxp = 0, mxpn = 0, nxpn = 0, lxpn = 0, mx = 0, nx = 0, lx = 0, my = 0, ny = 0, ly = 0, one = 1;
    int ns = 0, np = 0, m = 0, l = 0, i = 0, outmode = 0;
    int *dim = NULL, *str_outmode = NULL, *k = NULL, *ad = NULL;
    double **xp = NULL, **x = NULL, *val = NULL, *u = NULL, *v = NULL, *yp = NULL;
    RealHyperMat U;

    n = (Rhs + 1) / 2 - 1;
    if ( n < 1 )
    {
        Scierror(999, _("%s: Wrong number of input argument(s).\n"), fname);
        return 0;
    }

    /*  les points sur lesquels on evalue par interpolation */
    /*   l = I_UINT32; CreateVar(Rhs+1,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &n, &one, &l); */
    /*   xp = (double **) istk(l); */
    CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &n, &one, &l); /* => lets store an array of pointers  */
    xp = (double **) stk(l);               /*   with size of 4 or 8 bytes */

    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &mxp, &nxp, &lxp);

    {
        SciErr sciErr;
        int *piAddressVar = NULL;
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVar);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if (isVarComplex(pvApiCtx, piAddressVar))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), fname, 1);
            return 0;
        }
    }
    xp[0] = stk(lxp);
    np = mxp * nxp;
    for (i = 2 ; i <= n ; i++ )
    {
        SciErr sciErr;
        int *piAddressVar = NULL;

        GetRhsVar(i, MATRIX_OF_DOUBLE_DATATYPE, &mxpn, &nxpn, &lxpn);
        if ( mxp != mxpn || nxp != nxpn )
        {
            Scierror(999, _("%s: Wrong size for input arguments #%d and #%d: Same sizes expected.\n"), fname, 1, i);
            return 0;
        }

        sciErr = getVarAddressFromPosition(pvApiCtx, i, &piAddressVar);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, i);
            return 0;
        }

        if (isVarComplex(pvApiCtx, piAddressVar))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), fname, i);
            return 0;
        }

        xp[i - 1] = stk(lxpn);
    }

    /* coordonnées de la grille */
    l = I_INT32;
    CreateVar(Rhs + 2, MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &n, &one, &l);
    dim = istk(l);
    /*   l = I_UINT32; CreateVar(Rhs+3,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &n, &one, &l); */
    /*   x = (double **) istk(l); */
    CreateVar(Rhs + 3, MATRIX_OF_DOUBLE_DATATYPE, &n, &one, &l); /* => lets store an array of pointers  */
    x = (double **) stk(l);               /*    with size(void *) = 4 or 8 bytes */

    for ( i = 1 ; i <= n ; i++ )
    {
        GetRhsVar(n + i, MATRIX_OF_DOUBLE_DATATYPE, &mx, &nx, &lx);
        if ( (mx != 1 && nx != 1) && mx * nx < 2)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, n + i);
            return 0;
        }
        x[i - 1] = stk(lx);
        dim[i - 1] = mx * nx;
        /* verify strict increasing order  */
        if ( !good_order(x[i - 1], mx * nx) )
        {
            Scierror(999, _("%s: Grid abscissae of dim %d not in strict increasing order.\n"), fname, n + i);
            return 0;
        }
    }

    /* les valeurs de la grille */
    if ( n >= 3 )
    {
        GetRhsRealHMat(2 * n + 1, &U);
        if ( U.dimsize != n )
        {
            Scierror(999, _("%s: %s must be a real %d-dim hypermatrix.\n"), fname, "U", n);
            return 0;
        }
        for ( i = 0 ; i < n ; i++ )
            if ( U.dims[i] != dim[i] )
            {
                Scierror(999, _("%s: Size incompatibility between grid points and grid values in dimension %d.\n"), fname, i + 1);
                return 0;
            }
        val = U.R;
    }
    else  /* n = 1 or 2 */
    {
        GetRhsVar(2 * n + 1, MATRIX_OF_DOUBLE_DATATYPE, &my, &ny, &ly);
        if ( n == 1  &&  my*ny != dim[0] )
        {
            Scierror(999, _("%s: Size incompatibility between grid points and values in dimension %d.\n"), fname, 1);
            return 0;
        }
        if ( n == 2  &&  (my != dim[0]  || ny != dim[1]) )
        {
            Scierror(999, _("%s: Size incompatibility between grid points and values in dimension %d or %d.\n"), fname, 1, 2);
            return 0;
        }
        val = stk(ly);
    }

    /* get the outmode */
    if ( Rhs == 2 * n + 2 )
    {
        GetRhsScalarString(Rhs, &ns, &str_outmode);
        outmode =  get_type(OutModeTable, NB_OUTMODE, str_outmode, ns);
        if ( outmode == UNDEFINED || outmode == LINEAR )
        {
            Scierror(999, _("%s: Wrong values for input argument #%d: Unsupported '%s' type.\n"), fname, 2 * n + 2, "outmode");
            return 0;
        };
    }
    else
        outmode = C0;

    CreateVar(Rhs + 4, MATRIX_OF_DOUBLE_DATATYPE, &n, &one, &l);
    u = stk(l);
    m = 1;
    for ( i = 1 ; i <= n ; i++) m = 2 * m;
    CreateVar(Rhs + 5, MATRIX_OF_DOUBLE_DATATYPE, &m, &one, &l);
    v = stk(l);
    l = 4;
    CreateVar(Rhs + 6, MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &m, &one, &l);
    ad = istk(l);
    l = 4;
    CreateVar(Rhs + 7, MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &n, &one, &l);
    k = istk(l);
    CreateVar(Rhs + 8, MATRIX_OF_DOUBLE_DATATYPE, &mxp, &nxp, &l);
    yp = stk(l);

    nlinear_interp(x, val, dim, n, xp, yp, np, outmode, u, v, ad, k);

    LhsVar(1) = Rhs + 8;
    PutLhsVar();
    /* correction Warning Allan CORNET */
    /* warning C4715: 'intlinear_interpn' : not all control paths return a value */
    return 0;
}
/*--------------------------------------------------------------------------*/

