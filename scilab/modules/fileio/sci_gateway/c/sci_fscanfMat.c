/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2010 - 2011 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "api_scilab.h"
#include "MALLOC.h"
#include "gw_fileio.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "expandPathVariable.h"

#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "fscanfMat.h"
/*--------------------------------------------------------------------------*/
int sci_fscanfMat(char *fname,unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int m1 = 0, n1 = 0;
    int iType1 = 0;

    char *filename = NULL;
    char *expandedFilename = NULL;
    char *Format = NULL;
    char *separator = NULL;
    BOOL bIsDefaultSeparator = TRUE;

    fscanfMatResult *results = NULL;

    Nbvars = 0;
    CheckRhs(1, 3); 
    CheckLhs(1, 2);

    if (Rhs == 3)
    {
        int *piAddressVarThree = NULL;
        int m3 = 0, n3 = 0;
        int iType3 = 0;

        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
            return 0;
        }

        sciErr = getVarType(pvApiCtx, piAddressVarThree, &iType3);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
            return 0;
        }

        if (iType3 != sci_strings)
        {
            Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 3);
            return 0;
        }

        sciErr = getVarDimension(pvApiCtx, piAddressVarThree, &m3, &n3);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
            return 0;
        }

        if ( (m3 != n3) && (n3 != 1) ) 
        {
            Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 3);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddressVarThree, &separator))
        {
            Scierror(999,_("%s: Memory allocation error.\n"), fname);
            return 0;
        }
        bIsDefaultSeparator = FALSE;
    }
    else
    {
        bIsDefaultSeparator = TRUE;
    }

    if (Rhs >= 2)
    {
        int *piAddressVarTwo = NULL;
        int m2 = 0, n2 = 0;
        int iType2 = 0;

        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
        if(sciErr.iErr)
        {
            if (separator) {FREE(separator); separator = NULL;}
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        sciErr = getVarType(pvApiCtx, piAddressVarTwo, &iType2);
        if(sciErr.iErr)
        {
            if (separator) {FREE(separator); separator = NULL;}
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if (iType2 != sci_strings)
        {
            if (separator) {FREE(separator); separator = NULL;}
            Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
            return 0;
        }

        sciErr = getVarDimension(pvApiCtx, piAddressVarTwo, &m2, &n2);
        if(sciErr.iErr)
        {
            if (separator) {FREE(separator); separator = NULL;}
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if ( (m2 != n2) && (n2 != 1) ) 
        {
            if (separator) {FREE(separator); separator = NULL;}
            Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddressVarTwo, &Format))
        {
            if (separator) {FREE(separator); separator = NULL;}
            Scierror(999,_("%s: Memory allocation error.\n"), fname);
            return 0;
        }
    }
    else
    {
        Format = strdup(DEFAULT_FSCANFMAT_FORMAT);
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if(sciErr.iErr)
    {
        if (separator) {FREE(separator); separator = NULL;}
        if (Format) {FREE(Format); Format = NULL;}
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
    if(sciErr.iErr)
    {
        if (separator) {FREE(separator); separator = NULL;}
        if (Format) {FREE(Format); Format = NULL;}
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (iType1 != sci_strings)
    {
        if (separator) {FREE(separator); separator = NULL;}
        if (Format) {FREE(Format); Format = NULL;}
        Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarDimension(pvApiCtx, piAddressVarOne, &m1, &n1);
    if(sciErr.iErr)
    {
        if (separator) {FREE(separator); separator = NULL;}
        if (Format) {FREE(Format); Format = NULL;}
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if ( (m1 != n1) && (n1 != 1) ) 
    {
        if (separator) {FREE(separator); separator = NULL;}
        if (Format) {FREE(Format); Format = NULL;}
        Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &filename))
    {
        if (separator) {FREE(separator); separator = NULL;}
        if (Format) {FREE(Format); Format = NULL;}
        Scierror(999,_("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    expandedFilename = expandPathVariable(filename);
    if (bIsDefaultSeparator)
    {
        #define NB_DEFAULT_SUPPORTED_SEPARATORS 2

        /* bug 8148 */
        /* default separator can be a space or a tabulation */
        char *supportedSeparators[NB_DEFAULT_SUPPORTED_SEPARATORS] = {DEFAULT_FSCANFMAT_SEPARATOR, "\t"};
        int i = 0;

        for (i = 0; i < NB_DEFAULT_SUPPORTED_SEPARATORS; i++)
        {
            results = fscanfMat(expandedFilename, Format, supportedSeparators[i], TRUE);
            if (results && results->err == FSCANFMAT_NO_ERROR)  
            {
                break;
            }
        }
    }
    else
    {
        results = fscanfMat(expandedFilename, Format, separator, TRUE);
    }

    if (expandedFilename) {FREE(expandedFilename); expandedFilename = NULL;}
    if (Format) {FREE(Format); Format = NULL;}
    if (separator) {FREE(separator); separator = NULL;}

    if (results)
    {
        switch(results->err)
        {
        case FSCANFMAT_NO_ERROR:
            {
                if ( (results->values) && (results->m > 0) && (results->n > 0))
                {
                    sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, results->m, results->n, results->values);
                    if(sciErr.iErr)
                    {
                        freeFscanfMatResult(results);
                        results = NULL;
                        printError(&sciErr, 0);
                        return 0;
                    }
                    else
                    {
                        LhsVar(1) = Rhs + 1;
                    }
                }
                else
                {
                    if (createEmptyMatrix(pvApiCtx, Rhs + 1) == 0)
                    {
                        LhsVar(1) = Rhs + 1;
                    }
                }

                if (Lhs == 2)
                {
                    if (results->text)
                    {
                        sciErr = createMatrixOfString(pvApiCtx, Rhs + 2, results->sizeText, 1, results->text);

                        freeFscanfMatResult(results);
                        results = NULL;

                        if(sciErr.iErr)
                        {
                            printError(&sciErr, 0);
                            return 0;
                        }
                        else
                        {
                            LhsVar(2) = Rhs + 2;
                        }
                    }
                    else
                    {
                        char *emptryStr = strdup("");
                        if (emptryStr)
                        {
                            createSingleString(pvApiCtx, Rhs + 2, emptryStr);
                            FREE(emptryStr);
                            emptryStr = NULL;
                        }
                        LhsVar(2) = Rhs + 2;
                    }
                }

                freeFscanfMatResult(results);
                results = NULL;

                if (filename) {FREE(filename); filename = NULL;}

                PutLhsVar();
            }
            break;
        case FSCANFMAT_MOPEN_ERROR:
            {
                Scierror(999,_("%s: can not open file %s.\n"), fname, filename);
            }
            break;
        case FSCANFMAT_READLINES_ERROR:
            {
                Scierror(999,_("%s: can not read file %s.\n"), fname, filename);
            }
            break;
        case FSCANFMAT_FORMAT_ERROR:
            {
                Scierror(999,_("%s: Invalid format.\n"), fname);
            }
            break;
        case FSCANFMAT_MEMORY_ALLOCATION:
            {
                Scierror(999,_("%s: Memory allocation error.\n"), fname);
            }
            break;
        default:
        case FSCANFMAT_ERROR:
            {
                Scierror(999,_("%s: error.\n"), fname);
            }
            break;
        }
    }
    else
    {
        Scierror(999,_("%s: Memory allocation error.\n"), fname);
    }

    if (filename) {FREE(filename); filename = NULL;}

    return 0;
}
/*--------------------------------------------------------------------------*/
