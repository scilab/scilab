/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "gw_io.h"
#include "api_scilab.h"
#include "localization.h"
#include "sciprint.h"
#include "warningmode.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intload)(); /* fortran subroutine */
/*--------------------------------------------------------------------------*/
int sci_load(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int* piAddr = NULL;

    //check first input argument type ( double can be file descriptor )
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (isDoubleType(pvApiCtx, piAddr))
    {
        int Val = 40;
        int k1 = 0;
        if (getWarningMode())
        {
            sciprint(_("%s: Feature %s is obsolete.\n"), _("Warning"), "load(file_descriptor)");
            sciprint(_("%s: See help('load') for the rationale.\n"), _("Warning"));
            sciprint(_("%s: This feature will be permanently removed in Scilab %s\n\n"), _("Warning"), "6.0.0");
        }
        C2F(intload)(&Val, &k1);
    }
    else if (isStringType(pvApiCtx, piAddr))
    {
        int lw = 0;
        //call "overload" to prepare data to export_to_hdf5 function.
        C2F(overload) (&lw, "sodload", (unsigned long)strlen("sodload"));
    }
    else
    {
        int Val = 40;
        int k1 = 0;
        C2F(intload)(&Val, &k1);
    }

    return 0;
}

int sci_percent_load(char *fname, unsigned long fname_len)
{
    int Val = 40;
    int k1 = 0;
    C2F(intload)(&Val, &k1);
    return 0;
}
/*--------------------------------------------------------------------------*/
