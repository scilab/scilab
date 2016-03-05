/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Artem GLEBOV <aglebov@acm.org>
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

/* ==================================================================== */
extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"

    /**
    * The gateway function for soap_servers()
    * @param[in] fname the name of the file for the error messages
    * @return 0 if successful, a negative value otherwise
    */
    int sci_empty_test(char *fname, void* pvApiCtx)
    {
        SciErr sciErr;

        // allocate memory for values
        double dOut = 0;
        char *cOut = "zero";

        // this function does not take input arguments
        CheckInputArgument(pvApiCtx, 0, 0);

        // the number of output arguments must be 2
        CheckOutputArgument(pvApiCtx, 2, 2);

        // create results on stack
        sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 0, 0, &dOut);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 2, 0, 0, &cOut);

        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;

        return 0;
    }
    /* ==================================================================== */
}                               /* extern "C" */

/* ==================================================================== */
