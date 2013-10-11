/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA -
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <string.h>
#include "stack-c.h"
#include "CdfBase.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int  CdfBase(char *fname, int inarg, int oarg, int *callpos, char *option, char *errnames, int which, int (*fonc)(), void (*foncErr)() )
{
    int i = 0, status = 0, m[6], n[6], l[6];
    double bound = 0;
    if ( Rhs != inarg + 1 )
    {
        Scierror(999, _("%s: Wrong number of input arguments with the '%s' option: %d expected.\n"), fname, option, inarg + 1);
        return 1;
    }
    for ( i = 0 ; i < inarg ; i++ )
    {
        GetRhsVar(i + 2, MATRIX_OF_DOUBLE_DATATYPE, &m[i], &n[i], &l[i]);
    }
    for ( i = 1 ; i < inarg ; i++)
    {
        if ( m[i] != m[i - 1] || n[i] != n[i - 1])
        {
            Scierror(999, _("%s and %s must have same size.\n"), fname, errnames);
            return 1;
        }
    }
    for ( i = 0 ; i < oarg ; i++)
    {
        CreateVar(i + 2 + inarg, MATRIX_OF_DOUBLE_DATATYPE, &m[0], &n[0], &l[i + inarg]);
    }

    switch ( inarg + oarg)
    {
        case 5:
            for ( i = 0 ; i < m[0]*n[0]; i++)
            {
                (*fonc)(&which, stk(l[callpos[0]] + i), stk(l[callpos[1]] + i),
                        stk(l[callpos[2]] + i), stk(l[callpos[3]] + i),
                        stk(l[callpos[4]] + i),
                        &status, &bound);

                if (status != 0)
                {
                    (*foncErr)(status, bound);
                    return 1;
                }
            }
            break;

        case 6:
            for ( i = 0 ; i < m[0]*n[0]; i++)
            {
                (*fonc)(&which, stk(l[callpos[0]] + i), stk(l[callpos[1]] + i),
                        stk(l[callpos[2]] + i), stk(l[callpos[3]] + i),
                        stk(l[callpos[4]] + i), stk(l[callpos[5]] + i),
                        &status, &bound);

                if (status != 0)
                {
                    /** Scierror(999,"i=%d\n",i); **/
                    (*foncErr)(status, bound);
                    return 1;
                }
            }
            break;
        case 4:
            for ( i = 0 ; i < m[0]*n[0]; i++)
            {
                (*fonc)(&which, stk(l[callpos[0]] + i), stk(l[callpos[1]] + i),
                        stk(l[callpos[2]] + i), stk(l[callpos[3]] + i),
                        &status, &bound);

                if (status != 0)
                {
                    (*foncErr)(status, bound);
                    return 1;
                }
            }
            break;
    }

    for ( i = 0 ; i < oarg ; i++)
    {
        LhsVar(i + 1) = i + 2 + inarg;
    }

    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
void CdfDefaultError(char** param, int status, double bound)
{
    if (bound <= ZERO_FOR_CDF)
    {
        Scierror(999, "Input argument %c is out of range.\nBound exceeded: %f\n", param[-status - 1], 0);
    }
    else if (bound >= INFINITY_FOR_CDF)
    {
        Scierror(999, "Input argument %c is out of range.\nBound exceeded: %s\n", param[-status - 1], "%inf");
    }
    else
    {
        Scierror(999, "Input argument %c is out of range.\nBound exceeded: %f\n", param[-status - 1], bound);
    }
}
/*--------------------------------------------------------------------------*/
void cdfLowestSearchError(double bound)
{
    if (bound == ZERO_FOR_CDF)
    {
        Scierror(999, _("Answer appears to be lower than lowest search bound %g\n"), ZERO_FOR_CDF);
    }
    else
    {
        Scierror(999, _("Answer appears to be lower than lowest search bound %f\n"), bound);
    }
}
/*--------------------------------------------------------------------------*/
void cdfGreatestSearchError(double bound)
{
    if (bound >= INFINITY_FOR_CDF)
    {
        Scierror(999, _("Answer appears to be higher than greatest search bound %s\n"), "%inf");
    }
    else
    {
        Scierror(999, _("Answer appears to be higher than greatest search bound %f\n"), bound);
    }
}
/*--------------------------------------------------------------------------*/
