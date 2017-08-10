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
#include "api_scilab.h"
#include "gw_matio.h"

#include "CreateMatlabVariable.h"
#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"

enum matfile_errors
{
    NO_MORE_VARIABLES = -1,
    UNKNOWN_VARIABLE_TYPE = 0
};

int sci_matfile_varreadnext(char *fname, void* pvApiCtx)
{
    mat_t *matfile = NULL;
    matvar_t *matvar = NULL;
    int fileIndex = 0;
    int returnedClass = 0, var_type;
    int * fd_addr = NULL;
    double tmp_dbl;
    SciErr sciErr;

    CheckRhs(1, 1);
    CheckLhs(1, 3);

    /* Input argument is the index of the file to read */

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
            Scierror(999, _("%s: Wrong size for first input argument: Single double expected.\n"), fname);
            return FALSE;
        }
        fileIndex = (int)tmp_dbl;
    }
    else
    {
        Scierror(999, _("%s: Wrong type for first input argument: Double expected.\n"), fname);
        return FALSE;
    }

    /* Gets the corresponding matfile */
    matfile_manager(MATFILEMANAGER_GETFILE, &fileIndex, &matfile);

    if (matfile == NULL)
    {
        Scierror(999, _("%s: Invalid file identifier.\n"), fname);
        return FALSE;
    }

    matvar = Mat_VarReadNext(matfile);
    if ((matvar == NULL) || (matvar->name == NULL))
    {
        /* Return empty name */
        createSingleString(pvApiCtx, Rhs + 1, "\0");
        LhsVar(1) = Rhs + 1;

        if (Lhs >= 2)
        {
            /* Return empty value */
            createEmptyMatrix(pvApiCtx, Rhs + 2);
            LhsVar(2) = Rhs + 2;
        }

        if (Lhs == 3)
        {
            /* Return error flag instead of variable class */
            createScalarDouble(pvApiCtx, Rhs + 3, NO_MORE_VARIABLES);
            LhsVar(3) = Rhs + 3;
        }

        PutLhsVar();

        return TRUE;
    }

    /* To be sure isComplex is 0 or 1 */
    matvar->isComplex =  matvar->isComplex != 0;

    /* Return the variable name */
    createSingleString(pvApiCtx, Rhs + 1, matvar->name);
    LhsVar(1) = Rhs + 1;

    returnedClass = matvar->class_type;

    if (Lhs >= 2)
    {
        /* Return the values */
        if (!CreateMatlabVariable(pvApiCtx, Rhs + 2, matvar, NULL, -1)) /* Could not Create Variable */
        {
            sciprint("Do not know how to read a variable of class %d.\n", matvar->class_type);
            returnedClass = UNKNOWN_VARIABLE_TYPE;
        }
        LhsVar(2) = Rhs + 2;
    }

    if (Lhs == 3)
    {
        /* Create class return value */
        createScalarDouble(pvApiCtx, Rhs + 3, returnedClass);
        LhsVar(3) = Rhs + 3;
    }

    Mat_VarFree(matvar);
    PutLhsVar();
    return TRUE;
}
