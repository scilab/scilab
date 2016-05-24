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

#include "gw_matio.h"
#include "GetMatlabVariable.h"
#include "api_scilab.h"

enum matfile_errors
{
    NO_MORE_VARIABLES = -1,
    UNKNOWN_VARIABLE_TYPE = 0
};

int sci_matfile_varwrite(char *fname, void* pvApiCtx)
{
    int nbRow = 0, nbCol = 0;
    mat_t *matfile = NULL;
    matvar_t *matvar = NULL;
    int fileIndex = 0;
    char *varname = NULL;
    int flag = 0;
    int compressionFlag = 0;
    int var_type;
    int * fd_addr = NULL, * name_addr = NULL, * cp_flag_addr = NULL;
    double tmp_dbl;
    SciErr sciErr;

    CheckRhs(4, 4);
    CheckLhs(1, 1);

    /* Input argument is the index of the file to write */

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
        getScalarDouble(pvApiCtx, fd_addr, &tmp_dbl);
        if (!isScalar(pvApiCtx, fd_addr))
        {
            Scierror(999, _("%s: Wrong size for first input argument: An integer value expected.\n"), fname);
            return FALSE;
        }
        fileIndex = (int)tmp_dbl;
    }
    else
    {
        Scierror(999, _("%s: Wrong type for first input argument: An integer value expected.\n"), fname);
        return FALSE;
    }

    /* Gets the corresponding matfile */
    matfile_manager(MATFILEMANAGER_GETFILE, &fileIndex, &matfile);

    /* Second argument is the variable name */

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &name_addr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getVarType(pvApiCtx, name_addr, &var_type);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (var_type == sci_strings)
    {
        if (getAllocatedSingleString(pvApiCtx, name_addr, &varname) != 0)
        {
            return 0;
        }
        sciErr = getVarDimension(pvApiCtx, name_addr, &nbRow, &nbCol);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            freeAllocatedSingleString(varname);
            return 0;
        }
        if (nbCol != 1)
        {
            Scierror(999, _("%s: Wrong size for second input argument: string expected.\n"), fname);

            freeAllocatedSingleString(varname);

            return FALSE;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for second input argument: string expected.\n"), fname);

        freeAllocatedSingleString(varname);

        return FALSE;
    }

    /* Third argument is the variable data */
    /* TODO update this NULL */
    matvar = GetMatlabVariable(pvApiCtx, 3, varname, Mat_GetVersion(matfile), NULL, -1);

    /* Fourth argument is the compression flag */

    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &cp_flag_addr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getVarType(pvApiCtx, cp_flag_addr, &var_type);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (var_type == sci_boolean)
    {
        getScalarBoolean(pvApiCtx, cp_flag_addr, &compressionFlag);
        if (!isScalar(pvApiCtx, cp_flag_addr))
        {
            Scierror(999, _("%s: Wrong size for fourth input argument: A boolean expected.\n"), fname);

            freeAllocatedSingleString(varname);

            return FALSE;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for fourth input argument: A boolean expected.\n"), fname);

        freeAllocatedSingleString(varname);

        return FALSE;
    }

    flag = Mat_VarWrite(matfile, matvar, compressionFlag);

    /* Return execution flag */
    var_type = (flag == 0);
    createScalarBoolean(pvApiCtx, Rhs + 1, var_type);

    freeAllocatedSingleString(varname);
    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return TRUE;
}
