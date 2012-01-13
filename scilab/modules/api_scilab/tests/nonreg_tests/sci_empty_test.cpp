/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Artem GLEBOV <aglebov@acm.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* ==================================================================== */
extern "C"
{
#include "stack-c.h"
#include "api_scilab.h"
#include "Scierror.h"

    /**
     * The gateway function for soap_servers()
     * @param[in] fname the name of the file for the error messages
     * @return 0 if successful, a negative value otherwise
     */
    int sci_empty_test(char *fname)
    {
        SciErr sciErr;

        // this function does not take input arguments
          CheckRhs(0, 0);

        // the number of output arguments must be 2
          CheckLhs(2, 2);

        // allocate memory for values
        double dOut = 0;
        char *cOut = "zero";

          Rhs = Max(0, Rhs);

        // create results on stack
          sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 0, 0, &dOut);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        sciErr = createMatrixOfString(pvApiCtx, Rhs + 2, 0, 0, &cOut);

        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        LhsVar(1) = Rhs + 1;
        LhsVar(2) = Rhs + 2;

        // put the value on the stack
//        PutLhsVar();

        return 0;
    }
/* ==================================================================== */
}                               /* extern "C" */

/* ==================================================================== */
