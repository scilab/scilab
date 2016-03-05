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

#include "api_scilab.h"
#include "callfftw.h"
#include "Scierror.h"
#include "fftwlibname.h"
#include "gw_fftw.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_loadfftwlibrary(char *fname, void* pvApiCtx)
{
    char* FFTWLibname = NULL;
    char* l1char = NULL;
    int* piAddr1 = NULL;
    SciErr sciErr;
    int iErr = 0;

    CheckInputArgument(pvApiCtx, 1, 1);

    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (isStringType(pvApiCtx, piAddr1) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
        return 1;
    }

    // Retrieve a single string at position 1
    iErr = getAllocatedSingleString(pvApiCtx, piAddr1, &l1char);
    if (iErr)
    {
        freeAllocatedSingleString(l1char);
        return iErr;
    }
    FFTWLibname = l1char;
    setfftwlibname(FFTWLibname);

    if ( LoadFFTWLibrary(FFTWLibname) )
    {
        iErr = createScalarBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1); // true
    }
    else
    {
        iErr = createScalarBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 0); // false
    }
    freeAllocatedSingleString(l1char);

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
