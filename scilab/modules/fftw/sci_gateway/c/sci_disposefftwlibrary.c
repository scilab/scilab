/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
* Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
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

#include "callfftw.h"
#include "fftwlibname.h"
#include "gw_fftw.h"
#include "localization.h"
#include "api_scilab.h"
#include "Scierror.h"

/*--------------------------------------------------------------------------*/
int sci_disposefftwlibrary(char *fname, void* pvApiCtx)
{
    int iErr;

    if (DisposeFFTWLibrary())
    {
        iErr = createScalarBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1); // true
    }
    else
    {
        iErr = createScalarBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 0); // false
    }
    freefftwlibname();

    if (iErr)
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return iErr;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
