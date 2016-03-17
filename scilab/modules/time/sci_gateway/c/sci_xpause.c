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
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_xpause(char *fname, void* pvApiCtx)
{

    SciErr sciErr;
    int m1 = 0, n1 = 0, sec = 0;
    int * p1_in_address = NULL;
    double * pDblReal = NULL;

    sciprint(_("%s: Feature %s is obsolete and will be permanently removed in Scilab %s\n"), _("Warning"), fname, "6.1");
    sciprint(_("%s: Please use %s instead.\n"), _("Warning"), "sleep()");

    CheckLhs(0, 1);
    CheckRhs(1, 1);

    if (Rhs == 1)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &p1_in_address);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        sciErr = getMatrixOfDouble(pvApiCtx, p1_in_address, &m1, &n1, &pDblReal);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        if (isScalar(pvApiCtx, p1_in_address) == 0)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real scalar expected.\n"), fname, 1);
            return 0;
        }

        sec = (int)  * pDblReal;

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

