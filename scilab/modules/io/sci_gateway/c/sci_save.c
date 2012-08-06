/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "gw_io.h"
#include "stack-c.h"
#include "api_scilab.h"
#include "localization.h"
#include "sciprint.h"
#include "warningmode.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intsave)(); /* fortran subroutine */
/*--------------------------------------------------------------------------*/
int sci_save(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    int iOldSave    = FALSE;

    int* piAddr1    = NULL;
    int iType1      = 0;

    CheckRhs(1, 100000);
    CheckLhs(0, 1);

    //filename or file descriptor
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getVarType(pvApiCtx, piAddr1, &iType1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (iType1 == sci_strings)
    {
        int* piAddrI    = NULL;
        int* piAddrI2   = NULL;
        int iTypeI      = 0;
        int iRowsI      = 0;
        int iColsI      = 0;
        char* pstVarI   = NULL;

        if(Rhs > 1)
        {
            int i = 0;
            for (i = 2 ; i <= Rhs ; i++)
            {
                sciErr = getVarAddressFromPosition(pvApiCtx, i, &piAddrI);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 1;
                }

                sciErr = getVarType(pvApiCtx, piAddrI, &iTypeI);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 1;
                }

                if (iTypeI != sci_strings)
                {
                    iOldSave = TRUE;
                    break;
                }

                sciErr = getVarDimension(pvApiCtx, piAddrI, &iRowsI, &iColsI);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 1;
                }

                if (iRowsI != 1 || iColsI != 1)
                {
                    iOldSave = TRUE;
                    break;
                }

                if (getAllocatedSingleString(pvApiCtx, piAddrI, &pstVarI))
                {
                    return 1;
                }

                if(strcmp(pstVarI, "-append") != 0)
                {
                    //try to get variable by name
                    sciErr = getVarAddressFromName(pvApiCtx, pstVarI, &piAddrI2);
                    if (sciErr.iErr)
                    {
                        // Try old save because here the input variable can be of type "string" but not a variable name
                        // Ex: a=""; save(filename, a);
                        iOldSave = TRUE;
                        break;
                    }

                    if (piAddrI2 == 0)
                    {
                        iOldSave = TRUE;
                        break;
                    }
                }

                freeAllocatedSingleString(pstVarI);
            }
        }
        else
        {
            iOldSave = FALSE;
        }
    }
    else
    {
        iOldSave = TRUE;
    }

    //new save to sod format
    if(iOldSave == FALSE)
    {
        int lw = 0;
        //call "overload" to prepare data to export_to_hdf5 function.
        C2F(overload) (&lw, "save", (unsigned long)strlen("save"));
    }

    //old save

    if (iOldSave)
    {//show warning only for variable save, not for environment.
        if (getWarningMode() && Rhs > 1)
        {
            sciprint(_("%s: Scilab 6 will not support the file format used.\n"), _("Warning"));
            sciprint(_("%s: Please quote the variable declaration. Example, save('myData.sod',a) becomes save('myData.sod','a').\n"), _("Warning"));
            sciprint(_("%s: See help('save') for the rational.\n"), _("Warning"));
        }
        C2F(intsave)();
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
