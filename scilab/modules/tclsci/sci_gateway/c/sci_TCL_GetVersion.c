/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
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

#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "Scierror.h"
#include "localization.h"
#include "os_string.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
int sci_TCL_GetVersion(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    static int n1, m1;
    int major = 0;
    int minor = 0;
    int patchLevel = 0;
    int type = 0;
    char *output = NULL ;
    char VersionString[256];
    char ReleaseType[256];

    CheckInputArgument(pvApiCtx, 0, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    Tcl_GetVersion(&major, &minor, &patchLevel, &type);

    if (nbInputArgument(pvApiCtx) == 0)
    {
        switch (type)
        {
            case TCL_ALPHA_RELEASE:
                strcpy(ReleaseType, _("Alpha Release"));
                break;
            case TCL_BETA_RELEASE:
                strcpy(ReleaseType, _("Beta Release"));
                break;
            case TCL_FINAL_RELEASE:
                strcpy(ReleaseType, _("Final Release"));
                break;
            default:
                strcpy(ReleaseType, _("Unknown Release"));
                break;
        }

        sprintf(VersionString, "TCL/TK %d.%d.%d %s", major, minor, patchLevel, ReleaseType);
        output = os_strdup(VersionString);
        if (createSingleString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, output))
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        if (output)
        {
            FREE(output);
            output = NULL;
        }

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else
    {
        if (checkInputArgumentType(pvApiCtx, 1, sci_strings))
        {
            char *Param = NULL;

            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl1);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 1.
            if (getAllocatedSingleString(pvApiCtx, piAddrl1, &Param))
            {
                Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 1);
                return 1;
            }

            if (strcmp(Param, "numbers") == 0)
            {
                int *VERSIONMATRIX = NULL;
                VERSIONMATRIX = (int *)MALLOC( (4) * sizeof(int) );

                VERSIONMATRIX[0] = (int)major;
                VERSIONMATRIX[1] = (int)minor;
                VERSIONMATRIX[2] = (int)patchLevel;
                VERSIONMATRIX[3] = (int)type;

                m1 = 1;
                n1 = 4;
                sciErr = createMatrixOfDoubleAsInteger(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m1, n1 , VERSIONMATRIX);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    freeAllocatedSingleString(Param);
                    return 1;
                }

                FREE(VERSIONMATRIX);
                VERSIONMATRIX = NULL;
                AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
                ReturnArguments(pvApiCtx);
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: '%s' expected.\n"), fname, 1, "numbers");
            }

            freeAllocatedSingleString(Param);
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
        }

    }
    return 0;
}
/*--------------------------------------------------------------------------*/
