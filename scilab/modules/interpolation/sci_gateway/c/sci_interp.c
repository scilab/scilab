/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Bruno Pincon
 * Copyright (C) DIGITEO - 2012 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*/
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_interpolation.h"
#include "api_scilab.h"
#include "stack-c.h"
#include "interpolation.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
extern int C2F(evalpwhermite) (double *t, double *st, double *dst, double *ddst,
                               double *dddst, int *m, double *x, double *y, double *d, int *n, int *outmode);
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
int intinterp1(char *fname, unsigned long fname_len)
{
    int minrhs = 4, maxrhs = 5, minlhs = 1, maxlhs = 4;

    int mt = 0, nt = 0, lt = 0, mx = 0, nx = 0, lx = 0, my = 0, ny = 0, ly = 0, md = 0, nd = 0, ld = 0, ns = 0;
    int *str_outmode = NULL;
    int n = 0, m = 0, outmode = 0, lst = 0, ldst = 0, lddst = 0, ldddst = 0;
    int i = 0;

    CheckRhs(minrhs, maxrhs);
    CheckLhs(minlhs, maxlhs);

    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &mt, &nt, &lt);
    GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &mx, &nx, &lx);
    GetRhsVar(3, MATRIX_OF_DOUBLE_DATATYPE, &my, &ny, &ly);
    GetRhsVar(4, MATRIX_OF_DOUBLE_DATATYPE, &md, &nd, &ld);

    if ( mx != my  ||  nx != ny  ||  md != mx || nd != nx || (mx != 1  &&  nx != 1) || mx * nx < 2)
    {
        Scierror(999, _("%s: Wrong size for input arguments #%d and #%d: Same sizes expected.\n"), fname, 2, 3);
        return 0;
    }
    n = mx * nx;    /* number of interpolation points */
    m = mt * nt;    /* number of points to interpolate */

    for (i = 1; i <= minrhs; i++)
    {
        SciErr sciErr;
        int *piAddressVar = NULL;
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
    }

    if ( Rhs == 5 )   /* get the outmode */
    {
        GetRhsScalarString(5, &ns, &str_outmode);
        outmode = get_type(OutModeTable, NB_OUTMODE, str_outmode, ns);
        if ( outmode == UNDEFINED )
        {
            Scierror(999, _("%s: Wrong values for input argument #%d: Unknown '%s' type.\n"), fname, 5, "outmode");
            return 0;
        };
    }
    else
    {
        outmode = C0;  /* default outmode */
    }

    /* memory for st, dst, ddst, dddst */
    CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &mt,  &nt, &lst);
    CreateVar(Rhs + 2, MATRIX_OF_DOUBLE_DATATYPE, &mt,  &nt, &ldst);
    CreateVar(Rhs + 3, MATRIX_OF_DOUBLE_DATATYPE, &mt,  &nt, &lddst);
    CreateVar(Rhs + 4, MATRIX_OF_DOUBLE_DATATYPE, &mt,  &nt, &ldddst);

    /*      subroutine EvalPWHermite(t, st, dst, ddst, dddst, m, x, y, d, n, outmode)
    *      int m, n, outmode
    *      double precision t(m), st(m), dst(m), ddst(m), dddst(m), x(n), y(n), d(n)
    */
    C2F(evalpwhermite) (stk(lt), stk(lst), stk(ldst), stk(lddst), stk(ldddst),
                        &m, stk(lx), stk(ly), stk(ld), &n, &outmode);

    LhsVar(1) = Rhs + 1;
    LhsVar(2) = Rhs + 2;
    LhsVar(3) = Rhs + 3;
    LhsVar(4) = Rhs + 4;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
