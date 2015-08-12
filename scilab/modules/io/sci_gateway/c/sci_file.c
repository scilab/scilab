/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009-2010 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_io.h"
#include "api_scilab.h"
#include "FileExist.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
#include "filesmanagement.h"
#include "freeArrayOfString.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
#define FILE_OPEN_STR "open"
#define FILE_OLD_STR "old"
/*--------------------------------------------------------------------------*/
extern int C2F(intfile)(); /* fortran subroutine */
/*--------------------------------------------------------------------------*/
static int sci_file_no_rhs(char *fname);
static int sci_file_one_rhs(char *fname);
/*--------------------------------------------------------------------------*/
int sci_file(char *fname, unsigned long fname_len)
{
    if (Rhs == 0)
    {
        return sci_file_no_rhs(fname);
    }

    if (Rhs == 1)
    {
        return sci_file_one_rhs(fname);
    }

    if (Rhs >= 3)
    {
        SciErr sciErr;

        int *piAddressVarOne = NULL;
        int *piAddressVarTwo = NULL;
        int *piAddressVarThree = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
            return 0;
        }

        if ( isStringType(pvApiCtx, piAddressVarOne) &&
                isStringType(pvApiCtx, piAddressVarTwo)  &&
                isStringType(pvApiCtx, piAddressVarThree))
        {
            char *pStVarOne = NULL;
            int lenStVarOne = 0;

            wchar_t *pStVarTwo = NULL;
            int lenStVarTwo = 0;

            char *pStVarThree = NULL;
            int lenStVarThree = 0;

            int m1 = 0, n1 = 0;
            int m2 = 0, n2 = 0;
            int m3 = 0, n3 = 0;

            if (isScalar(pvApiCtx, piAddressVarOne) == 0)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
                return 0;
            }

            if (isScalar(pvApiCtx, piAddressVarTwo) == 0)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
                return 0;
            }

            if (isScalar(pvApiCtx, piAddressVarThree) == 0)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 3);
                return 0;
            }

            if (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &pStVarOne))
            {
                if (pStVarOne)
                {
                    freeAllocatedSingleString(pStVarOne);
                }

                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                return 0;
            }

            if (strcmp(pStVarOne, FILE_OPEN_STR) == 0)
            {
                FREE(pStVarOne);

                if (getAllocatedSingleString(pvApiCtx, piAddressVarThree, &pStVarThree))
                {
                    if (pStVarThree)
                    {
                        freeAllocatedSingleString(pStVarThree);
                    }

                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
                    return 0;
                }

                if (strcmp(pStVarThree, FILE_OLD_STR) == 0)
                {
                    FREE(pStVarThree);

                    if (getAllocatedSingleWideString(pvApiCtx, piAddressVarTwo, &pStVarTwo))
                    {
                        if (pStVarTwo)
                        {
                            freeAllocatedSingleWideString(pStVarTwo);
                        }

                        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
                        return 0;
                    }

                    if (!FileExistW(pStVarTwo))
                    {
                        if (Lhs == 2)
                        {
                            FREE(pStVarTwo);

                            if (createEmptyMatrix(pvApiCtx, Rhs + 1))
                            {
                                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                                return 0;
                            }

                            if (createScalarDouble(pvApiCtx, Rhs + 2, 240.))
                            {
                                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                                return 0;
                            }

                            LhsVar(2) = Rhs + 2;
                            LhsVar(1) = Rhs + 1;
                            PutLhsVar();
                        }
                        else
                        {
                            char *filename = wide_string_to_UTF8(pStVarTwo);
                            FREE(pStVarTwo);
                            if (filename)
                            {
                                Scierror(240, _("%s: The file \"%s\" does not exist.\n"), fname, filename);
                                FREE(filename);
                            }
                            else
                            {
                                Scierror(240, _("%s: The file does not exist.\n"), fname);
                            }
                        }
                        return 0;
                    }
                }
                else
                {
                    FREE(pStVarThree);
                }
            }
            else
            {
                FREE(pStVarOne);
            }
        }
    }

    C2F(intfile)();
    return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_file_no_rhs(char *fname)
{
    SciErr sciErr;
    int m_out = 0, n_out = 0;

    int sizeArray = 0;
    double *IdUsed = NULL;

    CheckLhs(0, 5);

    /* Lhs == 0 IDs*/
    IdUsed = GetFilesIdUsed(&sizeArray);

    if (IdUsed)
    {
        sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, sizeArray, IdUsed);
        FREE(IdUsed);

        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        LhsVar(1) = Rhs + 1;
    }

    if (Lhs > 1) /* Types */
    {
        char **TypeIdsAsString = GetTypesUsedAsString(&sizeArray);
        if (TypeIdsAsString)
        {
            sciErr = createMatrixOfString(pvApiCtx, Rhs + 2, 1, sizeArray, TypeIdsAsString);
            freeArrayOfString(TypeIdsAsString, sizeArray);

            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            LhsVar(2) = Rhs + 2;
        }
    }

    if (Lhs > 2) /* names */
    {
        char **Filenames = GetFilenamesUsed(&sizeArray);
        if (Filenames)
        {
            sciErr = createMatrixOfString(pvApiCtx, Rhs + 3, 1, sizeArray, Filenames);
            freeArrayOfString(Filenames, sizeArray);

            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            LhsVar(3) = Rhs + 3;
        }
    }

    if (Lhs > 3) /* mod */
    {
        double *Modes = GetModesUsed(&sizeArray);
        if (Modes)
        {
            sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 4, 1, sizeArray, Modes);
            FREE(Modes);

            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            LhsVar(4) = Rhs + 4;
        }
    }

    if (Lhs > 4) /* swap */
    {
        double *SwapId = GetSwapsUsed(&sizeArray);
        if (SwapId)
        {
            sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 5, 1, sizeArray, SwapId);
            FREE(SwapId);

            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            LhsVar(5) = Rhs + 5;
        }
    }

    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_file_one_rhs(char *fname)
{
    SciErr sciErr;
    int iErr = 0;
    int iID = 0;
    int *piAddressVarOne = NULL;
    double dVarOne = 0;

    /* get Address of inputs */
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (isDoubleType(pvApiCtx, piAddressVarOne) == 0 || isScalar(pvApiCtx, piAddressVarOne) == 0)
    {
        Scierror(201, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), fname, 1);
        return 0;
    }

    if (getScalarDouble(pvApiCtx, piAddressVarOne, &dVarOne))
    {
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    iID = (int)dVarOne;

    if (dVarOne != (double)iID)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: An integer expected.\n"), fname, 1);
        return 0;
    }

    /* Lhs = 0 ID */
    if (GetFileTypeOpenedInScilab(iID) != 0)
    {
        iErr = createScalarDouble(pvApiCtx, Rhs + 1, dVarOne);
    }
    else
    {
        iErr = createEmptyMatrix(pvApiCtx, Rhs + 1);
    }

    if (iErr)
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    LhsVar(1) = Rhs + 1;

    if (Lhs > 1) /* Type */
    {
        if (GetFileTypeOpenedInScilab(iID) != 0)
        {
            char *TypeIdAsString = GetFileTypeOpenedInScilabAsString(iID);
            iErr = createSingleString(pvApiCtx, Rhs + 2, TypeIdAsString);
            FREE(TypeIdAsString);
        }
        else
        {
            iErr = createEmptyMatrix(pvApiCtx, Rhs + 2);
        }

        if (iErr)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        LhsVar(2) = Rhs + 2;
    }

    if (Lhs > 2) /* name */
    {
        if (GetFileTypeOpenedInScilab(iID) != 0)
        {
            char *filename = NULL;
            if (GetFileNameOpenedInScilab(iID) == NULL)
            {
                filename = strdup("");
            }
            else
            {
                filename = strdup(GetFileNameOpenedInScilab(iID));
            }

            iErr = createSingleString(pvApiCtx, Rhs + 3, filename);
            FREE(filename);
        }
        else
        {
            iErr = createEmptyMatrix(pvApiCtx, Rhs + 3);
        }

        if (iErr)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        LhsVar(3) = Rhs + 3;
    }

    if (Lhs > 3)  /* mod */
    {
        if (GetFileTypeOpenedInScilab(iID) != 0)
        {
            double ModeId = (double)GetFileModeOpenedInScilab(iID);
            iErr = createScalarDouble(pvApiCtx, Rhs + 4, ModeId);
        }
        else
        {
            iErr = createEmptyMatrix(pvApiCtx, Rhs + 4);
        }

        if (iErr)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        LhsVar(4) = Rhs + 4;
    }

    if (Lhs > 4) /* swap */
    {
        if (GetFileTypeOpenedInScilab(iID) != 0)
        {
            double SwapId = (double)GetSwapStatus(iID);
            iErr = createScalarDouble(pvApiCtx, Rhs + 5, SwapId);
        }
        else
        {
            iErr = createEmptyMatrix(pvApiCtx, Rhs + 5);
        }

        if (iErr)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }
        LhsVar(5) = Rhs + 5;
    }

    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
