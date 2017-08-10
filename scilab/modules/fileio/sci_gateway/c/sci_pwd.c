/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
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
#include <string.h>
#include "gw_fileio.h"
#include "scicurdir.h"
#include "sci_malloc.h"
#include "Scierror.h"
#include "localization.h"
#include "api_scilab.h"

/*--------------------------------------------------------------------------*/
int sci_pwd(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int ierr = 0;
    char *path = NULL;

    CheckRhs(0, 0);
    CheckLhs(0, 1);

    path = scigetcwd(&ierr);

    if (ierr)
    {
        if (path)
        {
            FREE(path);
            path = NULL;
        }
        Scierror(998, _("%s: An error occurred.\n"), fname);
        return 0;
    }
    else
    {
        int n1 = 1;
        int m1 = (int)strlen(path);

        n1 = 1;
        sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, 1, 1, (char const * const*) &path);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            FREE(path);
            path = NULL;
            return 0;
        }

        LhsVar(1) = Rhs + 1;
        FREE(path);
        path = NULL;
        PutLhsVar();
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
