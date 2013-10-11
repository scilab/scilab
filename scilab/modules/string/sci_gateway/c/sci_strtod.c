/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*----------------------------------------------------------------------------*/
#include <stdlib.h>
#include "gw_string.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "api_scilab.h"
#include "locale.h"
#include "freeArrayOfString.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*----------------------------------------------------------------------------*/
int sci_strtod(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int* piAddr = NULL;
    int* piAddr2 = NULL;
    int iRows = 0, iCols = 0;
    int iRowsiCols = 0;
    char **Input_StringMatrix_1 = NULL;
    char *Input_SingleString_2 = NULL;
    int first_nb = 0;
    int x, y; //loop indexes
    char keys[] = "1234567890";
    char symbol1[] = "-+.";
    char symbol2[] = "-+,";
    unsigned long long raw = 0x7ff8000000000000;
    double not_a_number = *( double* )&raw;
    int iRhs = nbInputArgument(pvApiCtx);
    int iLhs = nbOutputArgument(pvApiCtx);
    int flag = 0;

    //output values
    double *OutputDoubles = NULL;
    char **OutputStrings = NULL;

    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 1, 2);

    if (iRhs == 2)
    {
        //get variable address
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }
        if (isStringType(pvApiCtx, piAddr2) == 0 || isScalar(pvApiCtx, piAddr2) == 0)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Single string expected.\n"), fname, 2);
            return 0;
        }
        if (getAllocatedSingleString(pvApiCtx, piAddr2, &Input_SingleString_2))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Single string expected.\n"), fname, 2);
            return 0;
        }
        //Test on optional argument value
        if (Input_SingleString_2[0] != '.' && Input_SingleString_2[0] != ',')
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: '.' or ',' expected.\n"), fname, 2);
        }
    }
    else
    {
        Input_SingleString_2 = strdup(".");
    }

    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        FREE(Input_SingleString_2);
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (isEmptyMatrix(pvApiCtx, piAddr))
    {
        FREE(Input_SingleString_2);
        if (createEmptyMatrix(pvApiCtx, iRhs + 1) != 0)
        {
            return 0;
        }

        AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;

        if (iLhs == 2)
        {
            if (createSingleString(pvApiCtx, iRhs + 2, ""))
            {
                return 0;
            }

            AssignOutputVariable(pvApiCtx, 2) = iRhs + 2;
        }

        ReturnArguments(pvApiCtx);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddr) == 0) //Check type
    {
        FREE(Input_SingleString_2);
        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings or empty matrix expected.\n"), fname, 1);
        return 0;
    }

    if (getAllocatedMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, &Input_StringMatrix_1))
    {
        if (Input_StringMatrix_1)
        {
            freeAllocatedMatrixOfString(iRows, iCols, Input_StringMatrix_1);
        }

        FREE(Input_SingleString_2);
        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings or empty matrix expected.\n"), fname, 1);
        return 0;
    }

    iRowsiCols = iRows * iCols;

    if (iLhs == 2)
    {
        OutputStrings = (char **)MALLOC(sizeof(char*) * iRowsiCols);
        if (OutputStrings == NULL)
        {
            freeAllocatedMatrixOfString(iRows, iCols, Input_StringMatrix_1);
            freeAllocatedSingleString(Input_SingleString_2);
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
    }

    sciErr = allocMatrixOfDouble(pvApiCtx, iRhs + 1, iRows, iCols, &OutputDoubles);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    for (x = 0 ; x < iRowsiCols ; x++)
    {
        //Double part
        char *stopstring = NULL;
        int iSign = 0;

        if (Input_SingleString_2[0] == ',')
        {
            iSign = (int)strcspn(Input_StringMatrix_1[x], symbol2);
        }
        else if (Input_SingleString_2[0] == '.')
        {
            iSign = (int)strcspn(Input_StringMatrix_1[x], symbol1);
        }
        first_nb = (int)strcspn(Input_StringMatrix_1[x], keys);

        //symbol can be use only if it is before key
        if (iSign == first_nb - 1)
        {
            //let strtod do with symbol
            first_nb -= 1;
        }

        //special case for "-.3"
        if (iSign == first_nb - 2 && (Input_StringMatrix_1[x][iSign + 1] == '.' || Input_StringMatrix_1[x][iSign + 1] == ','))
        {

            //let strtod do with symbol
            first_nb -= 2;
        }

        //Check if there is a number in the string
        if (first_nb != 0)
        {
            flag = 0;

            for (y = 0 ; y < first_nb ; y++)
            {
                if (Input_StringMatrix_1[x][y] != ' ') // spaces are accepted
                {
                    OutputDoubles[x] = not_a_number;
                    flag = 1;
                    stopstring = Input_StringMatrix_1[x];
                    break;
                }
            }

            //it is still a number
            if (flag == 0)
            {
                //only spaces ?
                if (strlen(Input_StringMatrix_1[x]) == first_nb) // strtod("  ")
                {
                    OutputDoubles[x] = not_a_number;
                    stopstring = Input_StringMatrix_1[x];
                }
                else // strtod("  000xxx")
                {
                    if (Input_SingleString_2[0] == ',')
                    {
#ifdef _MSC_VER
                        setlocale(LC_NUMERIC, "French_France.1252");
#else
                        setlocale(LC_NUMERIC, "fr_FR.UTF-8");
#endif
                        OutputDoubles[x] = (double)strtod(Input_StringMatrix_1[x] + first_nb, &stopstring);
                        setlocale(LC_NUMERIC, "C");
                    }
                    else
                    {
                        OutputDoubles[x] = (double)strtod(Input_StringMatrix_1[x] + first_nb, &stopstring);
                    }
                }
            }
        }
        else if (strlen(Input_StringMatrix_1[x]) == 0) //case strtod("")
        {
            OutputDoubles[x] = not_a_number;
        }
        else //all characters are digits
        {
            if (Input_SingleString_2[0] == ',')
            {
#ifdef _MSC_VER
                setlocale(LC_NUMERIC, "French_France.1252");
#else
                setlocale(LC_NUMERIC, "fr_FR.UTF-8");
#endif
                OutputDoubles[x] = (double)strtod(Input_StringMatrix_1[x], &stopstring);
                setlocale(LC_NUMERIC, "C");
            }
            else
            {
                OutputDoubles[x] = (double)strtod(Input_StringMatrix_1[x], &stopstring);
            }
        }

        if (iLhs == 2)
        {
            //String part
            if (stopstring)
            {
                OutputStrings[x] = (char*)MALLOC(sizeof(char) * (strlen(stopstring) + 1));
            }
            else
            {
                OutputStrings[x] = (char*)MALLOC(sizeof(char) * (strlen("") + 1));
            }

            if (OutputStrings[x] == NULL)
            {
                freeAllocatedMatrixOfString(iRows, iCols, Input_StringMatrix_1);
                freeAllocatedSingleString(Input_SingleString_2);
                freeAllocatedMatrixOfString(iRows, iCols, OutputStrings);
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }

            if (stopstring)
            {
                if (Input_SingleString_2[0] == ',')
                {
                    strcpy(OutputStrings[x], stopstring);
                }
                else
                {
                    strcpy(OutputStrings[x], stopstring);
                }
            }
            else
            {
                strcpy(OutputStrings[x], "");
            }
        }
    }

    AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;

    if (iLhs == 2)
    {
        sciErr = createMatrixOfString(pvApiCtx, iRhs + 2, iRows, iCols, OutputStrings);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        AssignOutputVariable(pvApiCtx, 2) = iRhs + 2;
        freeArrayOfString(OutputStrings, iRows * iCols);
    }

    freeAllocatedMatrixOfString(iRows, iCols, Input_StringMatrix_1);
    freeAllocatedSingleString(Input_SingleString_2);
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/

