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
#include "matfile_manager.h"
#include "gw_matio.h"
#include "localization.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "freeArrayOfString.h"
#include "os_string.h"

int sci_matfile_listvar(char *fname, void* pvApiCtx)
{
    int nbRow = 0, nbCol = 0;
    mat_t *matfile = NULL;
    matvar_t *matvar = NULL;
    int fileIndex = 0;
    char **varnames = NULL;
    double *varclasses = NULL;
    double *vartypes = NULL;
    int nbvar = 0, var_type;
    int * fd_addr = NULL;
    double tmp_dbl;
    SciErr sciErr;
    int iErr = 0;

    CheckRhs(1, 1);
    CheckLhs(1, 3);

    /* First Rhs is the index of the file to read */

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

    /* Back to the beginning of the file */
    if (Mat_Rewind(matfile) != 0)
    {
        Scierror(999, _("%s: Could not rewind the file %s.\n"), "matfile_listvar", Mat_GetFilename(matfile));
        return FALSE;
    }

    matvar = Mat_VarReadNext(matfile);
    while (matvar != NULL && matvar->name != NULL)
    {
        nbvar++;
        varnames = (char**) REALLOC(varnames, nbvar * sizeof(char*));
        if (varnames == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), "matfile_listvar");
            FREE(varclasses);
            FREE(vartypes);
            return FALSE;
        }
        varnames[nbvar - 1] = os_strdup(matvar->name);
        varclasses = (double*) REALLOC(varclasses, nbvar * sizeof(double));
        if (varclasses  == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), "matfile_listvar");
            FREE(vartypes);
            FREE(varnames);
            return FALSE;
        }
        varclasses[nbvar - 1] = (double) matvar->class_type;
        vartypes = (double*) REALLOC(vartypes, nbvar * sizeof(double));
        if (vartypes == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), "matfile_listvar");
            FREE(varnames);
            FREE(varclasses);
            return FALSE;
        }
        vartypes[nbvar - 1] = (double) matvar->data_type;

        Mat_VarFree(matvar);
        matvar = Mat_VarReadNext(matfile);
    }

    Mat_VarFree(matvar);

    /* Return the variable names list */
    if (nbvar == 0)
    {
        /* No variables found in MatFile */

        /* Return the empty names */
        iErr = createEmptyMatrix(pvApiCtx, Rhs + 1);
        if (iErr)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }
        LhsVar(1) = Rhs + 1;

        /* Return the empty classes if asked */
        if (Lhs >= 2)
        {
            iErr = createEmptyMatrix(pvApiCtx, Rhs + 2);
            if (iErr)
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }
            LhsVar(2) = Rhs + 2;
        }

        /* Return the empty types if asked */
        if (Lhs >= 3)
        {
            iErr = createEmptyMatrix(pvApiCtx, Rhs + 3);
            if (iErr)
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }
            LhsVar(3) = Rhs + 3;
        }
    }
    else
    {
        nbRow = nbvar;
        nbCol = 1;
        sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, nbRow, nbCol, varnames);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        LhsVar(1) = Rhs + 1;

        /* Return the variable classes */
        if (Lhs >= 2)
        {
            sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 2, nbRow, nbCol, varclasses);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
            LhsVar(2) = Rhs + 2;
        }

        /* Return the variable types */
        if (Lhs >= 3)
        {
            sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 3, nbRow, nbCol, vartypes);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
            LhsVar(3) = Rhs + 3;
        }
    }

    freeArrayOfString(varnames, nbvar);
    FREE(varclasses);
    FREE(vartypes);

    PutLhsVar();

    return TRUE;
}
