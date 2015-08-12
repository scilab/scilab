/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
* Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#include "api_scilab.h"
#include "callfftw.h"
#include "Scierror.h"
#include "fftwlibname.h"
#include "gw_fftw.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_loadfftwlibrary(char *fname, unsigned long fname_len)
{
    static int l1, n1, m1;
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
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
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

    n1 = 1;
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
