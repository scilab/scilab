/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Paul Bignier: bug #13102 fixed
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
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
#include "sci_malloc.h"
#include "matfile_manager.h"
#include "gw_matio.h"
#include "localization.h"
#include "Scierror.h"
#include <string.h>

/*******************************************************************************
   Interface for MATIO function called Mat_Open
   Scilab function name : matfile_open
*******************************************************************************/
int sci_matfile_open(char *fname, void* pvApiCtx)
{
    int nbRow = 0, nbCol = 0;
    mat_t *matfile;
    int fileIndex = 0;
    char * filename  = NULL;
    char * optionStr = NULL;
    int option = 0, var_type;
    int * filename_addr = NULL, * option_addr = NULL, * version_addr = NULL;
    char * versionStr = NULL;
    int version = MAT_FT_MAT5; // By default, use MAtlab 5 files
    SciErr sciErr;

    CheckRhs(1, 3);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &filename_addr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getVarType(pvApiCtx, filename_addr, &var_type);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (var_type == sci_strings)
    {
        if (getAllocatedSingleString(pvApiCtx, filename_addr, &filename) != 0)
        {
            return 0;
        }
        sciErr = getVarDimension(pvApiCtx, filename_addr, &nbRow, &nbCol);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            freeAllocatedSingleString(filename);
            return 0;
        }

        if (nbCol != 1)
        {
            Scierror(999, _("%s: Wrong size for first input argument: string expected.\n"), fname);
            freeAllocatedSingleString(filename);
            return FALSE;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for first input argument: string expected.\n"), fname);
        freeAllocatedSingleString(filename);
        return FALSE;
    }

    if (Rhs >= 2)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &option_addr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            freeAllocatedSingleString(filename);
            return 0;
        }

        sciErr = getVarType(pvApiCtx, option_addr, &var_type);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            freeAllocatedSingleString(filename);
            return 0;
        }

        if (var_type == sci_strings)
        {
            if (getAllocatedSingleString(pvApiCtx, option_addr, &optionStr) != 0)
            {
                freeAllocatedSingleString(filename);
                return 0;
            }
            sciErr = getVarDimension(pvApiCtx, option_addr, &nbRow, &nbCol);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                freeAllocatedSingleString(filename);
                freeAllocatedSingleString(optionStr);
                return 0;
            }

            if (nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for second input argument: string expected.\n"), fname);
                freeAllocatedSingleString(filename);
                freeAllocatedSingleString(optionStr);

                return FALSE;
            }

            if (strcmp(optionStr, "r") == 0)
            {
                option = MAT_ACC_RDONLY;
            }
            else if (strcmp(optionStr, "w") == 0)
            {
                option = MAT_ACC_RDWR;
            }
            else
            {
                Scierror(999, _("%s: Wrong value for second input argument: 'r' or 'w' expected.\n"), fname);
                freeAllocatedSingleString(filename);
                freeAllocatedSingleString(optionStr);

                return FALSE;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for second input argument: string expected.\n"), fname);
            freeAllocatedSingleString(filename);
            freeAllocatedSingleString(optionStr);

            return FALSE;
        }
    }
    else
    {
        /* Default option value */
        option = MAT_ACC_RDONLY;
    }

    if (Rhs >= 3)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &version_addr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            freeAllocatedSingleString(filename);
            freeAllocatedSingleString(optionStr);
            return 0;
        }

        sciErr = getVarType(pvApiCtx, version_addr, &var_type);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            freeAllocatedSingleString(filename);
            freeAllocatedSingleString(optionStr);
            return 0;
        }
        if (var_type == sci_strings)
        {
            if (getAllocatedSingleString(pvApiCtx, version_addr, &versionStr) != 0)
            {
                freeAllocatedSingleString(filename);
                freeAllocatedSingleString(optionStr);
                return 0;
            }
            sciErr = getVarDimension(pvApiCtx, version_addr, &nbRow, &nbCol);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                freeAllocatedSingleString(filename);
                freeAllocatedSingleString(optionStr);
                freeAllocatedSingleString(versionStr);
                return 0;
            }
            if (nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), fname, 3);
                freeAllocatedSingleString(filename);
                freeAllocatedSingleString(optionStr);
                freeAllocatedSingleString(versionStr);

                return FALSE;
            }

            if (strcmp(versionStr, "7.3") == 0)
            {
                version = MAT_FT_MAT73; // Matlab 7.3 file
            }
            else
            {
                version = MAT_FT_MAT5; // Default, Matlab 5 file
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 3);
            freeAllocatedSingleString(filename);
            freeAllocatedSingleString(optionStr);
            return 0;
        }
    }

    if (option == MAT_ACC_RDWR) // Write, option = "w"
    {
        /* create a Matlab 5 or 7.3 file */
        matfile = Mat_CreateVer(filename, NULL, version);
    }
    else // Read, option = "r"
    {
        /* Try to open the file (as a Matlab 5 file) */
        matfile = Mat_Open(filename, option);
    }

    if (matfile == NULL) /* Opening failed */
    {
        /* Function returns -1 */
        fileIndex = -1;
    }

    if (matfile != NULL) /* Opening succeed */
    {
        /* Add the file to the manager */
        matfile_manager(MATFILEMANAGER_ADDFILE, &fileIndex, &matfile);
    }

    /* Return the index */
    createScalarDouble(pvApiCtx, Rhs + 1, (double)fileIndex);

    freeAllocatedSingleString(filename);
    freeAllocatedSingleString(optionStr);
    freeAllocatedSingleString(versionStr);

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return TRUE;
}
