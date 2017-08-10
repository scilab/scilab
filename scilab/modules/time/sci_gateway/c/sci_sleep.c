/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_sleep(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int m1 = 0, n1 = 0 , iRows = 0, iCols = 0, option = 0;
    double sec = 0;
    char * opt = NULL;
    int * p1_in_address = NULL;
    int * p2_in_address = NULL;
    double * pDblReal = NULL;


    CheckLhs(0, 1);
    CheckRhs(1, 2);

    if (Rhs >= 1)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &p1_in_address);
        sciErr = getMatrixOfDouble(pvApiCtx, p1_in_address, &m1, &n1, &pDblReal);

        if (isScalar(pvApiCtx, p1_in_address) == 0)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real scalar expected.\n"), fname, 1);
            return 0;
        }
        sec = (double)  * pDblReal;

        if (sec <= 0)
        {
            Scierror(999, _("%s: Wrong values for input argument #%d: Non-negative integers expected.\n"), fname, 1);
            return 0;
        }

        if (Rhs == 2)
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 2, &p2_in_address);
            if (getAllocatedSingleString(pvApiCtx, p2_in_address, &opt))
            {
                return 0;
            }

            if (strcmp("s", opt) != 0)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: 's' expected.\n"), fname, 2);
                freeAllocatedSingleString(opt);
                return 0;
            }
            if (strcmp("s", opt) == 0)
            {
                option = 1;
            }
            freeAllocatedSingleString(opt);
        }


#ifdef _MSC_VER
        {
            double ms = 0;
            if (option == 1)
            {
                ms = sec * 1000; /* convert seconds into milliseconds */
            }
            else
            {
                ms = sec;
            }

            if (ms > 0)
            {
                Sleep((DWORD)ms);    /* Number of milliseconds to sleep. */
            }
        }
#else
        {
            unsigned useconds;
            useconds = (unsigned) sec;
            if (option == 1)
            {
                useconds = useconds * 1000;
            }
            if (useconds != 0)
#ifdef HAVE_USLEEP
            {
                usleep(useconds * 1000);
            }
#else
#ifdef HAVE_SLEEP
            {
                sleep(useconds * 1000);
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

