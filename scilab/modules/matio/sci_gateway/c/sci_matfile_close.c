/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

#include "matfile_manager.h"
#include "localization.h"
#include "gw_matio.h"
#include "Scierror.h"
#include "sciprint.h"

#include "api_scilab.h"

/*******************************************************************************
Interface for MATIO function called Mat_Close
Scilab function name : matfile_close
*******************************************************************************/
int sci_matfile_close(char *fname, void* pvApiCtx)
{
    mat_t * matfile = NULL;
    int fileIndex = 0;
    int nbRow = 0, nbCol = 0;
    int * fd_addr = NULL;
    int flag = 1, var_type;
    double * fd_val = NULL;
    SciErr sciErr;

    CheckRhs(1, 1);
    CheckLhs(1, 1);

    /* First Rhs is the index of the file to close */
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &fd_addr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getVarType(pvApiCtx, fd_addr, &var_type);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (var_type == sci_matrix)
    {
        sciErr = getMatrixOfDouble(pvApiCtx, fd_addr, &nbRow, &nbCol, &fd_val);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        if (nbRow * nbCol != 1)
        {
            Scierror(999, _("%s: Wrong size for first input argument: Single double expected.\n"), fname);
            return FALSE;
        }
        fileIndex = (int) * fd_val;
    }
    else
    {
        Scierror(999, _("%s: Wrong type for first input argument: Double expected.\n"), fname);
        return FALSE;
    }

    /* Gets the corresponding matfile to close it */
    /* The manager clears its static matfile table */
    matfile_manager(MATFILEMANAGER_DELFILE, &fileIndex, &matfile);

    /* If the file has not already been closed, it's closed */
    if (matfile != NULL)
    {
        flag = Mat_Close(matfile);
    }
    else /* The user is informed */
    {
        sciprint("File already closed.\n");
    }

    /* Return execution flag */
    var_type = (flag == 0);
    createScalarBoolean(pvApiCtx, Rhs + 1, var_type);

    LhsVar(1) = Rhs + 1;

    PutLhsVar();

    return TRUE;
}
