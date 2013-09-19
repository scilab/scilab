/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <Windows.h>
#endif
#include "machine.h"
#ifdef HAVE_USLEEP
#include <unistd.h>
#endif
#include "gw_time.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_xpause(char *fname, unsigned long fname_len)
{
    int m1 = 0, n1 = 0, l1 = 0, sec = 0;

    CheckLhs(0, 1);
    CheckRhs(1, 1);
    if (Rhs == 1)
    {
        GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
        CheckScalar(1, m1, n1);
        sec = (int)  * stk(l1);
        if (sec <= 0)
        {
            Scierror(999, _("%s: Wrong values for input argument #%d: Non-negative integers expected.\n"), fname, 1);
            return 0;
        }

#ifdef _MSC_VER
        {
            int ms = (sec) / 1000; /** time is specified in milliseconds in scilab**/
            if (ms > 0)
            {
                Sleep(ms);    /* Number of milliseconds to sleep. */
            }
        }
#else
        {
            unsigned useconds;
            useconds = (unsigned) sec;
            if (useconds != 0)
#ifdef HAVE_USLEEP
            {
                usleep(useconds);
            }
#else
#ifdef HAVE_SLEEP
            {
                sleep(useconds);
            }
#endif
#endif
        }
#endif
    }
    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
