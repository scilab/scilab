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
#include "callfftw.h"
#include "gw_fftw.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"

/*--------------------------------------------------------------------------*/
int sci_fftwlibraryisloaded(char *fname, unsigned long fname_len)
{
    int iErr;

    if ( IsLoadedFFTW() )
    {
        iErr = createScalarBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1); // true
    }
    else
    {
        iErr = createScalarBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 0); // false
    }

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
