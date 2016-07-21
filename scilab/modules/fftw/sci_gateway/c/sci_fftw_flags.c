/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006/2007 - INRIA - Alan LAYEC
* Copyright (C) 2007 - INRIA - Allan CORNET
* Copyright (C) 2012 - DIGITEO - Allan CORNET
* Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
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
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "api_scilab.h"
#include "fftw_utilities.h"
#include "sci_malloc.h"
#include "gw_fftw.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "Scierror.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
/* fftw_flags function.
*
* Scilab Syntax :
*   -->[a,b]=fftw_flags();
* or
*   -->[a,b]=fftw_flags(S);
*
*  a is an int scalar. (the int value of
*                            the flag parameter of fftw)
*  b is a string matrix.(the string values of
*                             the flag parameter of fftw)
*  S is a string matrix or an int or a double scalar
*  given the value(s) of the fftw flag parameter.
*
*  This function gives and set the flag parameter of fftw
*  when creating a new plan.
*  This should be done before calling fftw function.
*  (default is FFTW_ESTIMATE)
*/
/*--------------------------------------------------------------------------*/
int sci_fftw_flags(char *fname,  void* pvApiCtx)
{
    /* declaration of variables to store scilab parameters address */
    static int m1 = 0, n1 = 0;

    char **Str1 = NULL;
    char **Str3 = NULL;

    unsigned int uiVar1 = 0;
    int* piDataOut = NULL;
    int* piAddr1 = NULL;
    int* piLen = NULL;
    int iType = 0;

    /* please update me ! */
    static int nb_flag = 22;
    static char *Str[] =
    {
        /* documented flags */
        "FFTW_MEASURE",
        "FFTW_DESTROY_INPUT",
        "FFTW_UNALIGNED",
        "FFTW_CONSERVE_MEMORY",
        "FFTW_EXHAUSTIVE",
        "FFTW_PRESERVE_INPUT",
        "FFTW_PATIENT",
        "FFTW_ESTIMATE",

        /* undocumented beyond-guru flags */
        "FFTW_ESTIMATE_PATIENT",
        "FFTW_BELIEVE_PCOST",
        "FFTW_NO_DFT_R2HC",
        "FFTW_NO_NONTHREADED",
        "FFTW_NO_BUFFERING",
        "FFTW_NO_INDIRECT_OP",
        "FFTW_ALLOW_LARGE_GENERIC",
        "FFTW_NO_RANK_SPLITS",
        "FFTW_NO_VRANK_SPLITS",
        "FFTW_NO_VRECURSE",
        "FFTW_NO_SIMD",
        "FFTW_NO_SLOW",
        "FFTW_NO_FIXED_RADIX_LARGE_N",
        "FFTW_ALLOW_PRUNING"
    };

    static unsigned flagt[] =
    {
        /* documented flags */
        FFTW_MEASURE,
        FFTW_DESTROY_INPUT,
        FFTW_UNALIGNED,
        FFTW_CONSERVE_MEMORY,
        FFTW_EXHAUSTIVE,
        FFTW_PRESERVE_INPUT,
        FFTW_PATIENT,
        FFTW_ESTIMATE,

        /* undocumented beyond-guru flags */
        FFTW_ESTIMATE_PATIENT,
        FFTW_BELIEVE_PCOST,
        FFTW_NO_DFT_R2HC,
        FFTW_NO_NONTHREADED,
        FFTW_NO_BUFFERING,
        FFTW_NO_INDIRECT_OP,
        FFTW_ALLOW_LARGE_GENERIC,
        FFTW_NO_RANK_SPLITS,
        FFTW_NO_VRANK_SPLITS,
        FFTW_NO_VRECURSE,
        FFTW_NO_SIMD,
        FFTW_NO_SLOW,
        FFTW_NO_FIXED_RADIX_LARGE_N,
        FFTW_ALLOW_PRUNING
    };

    unsigned flagv = 0;

    int i = 0, j = 0;

    SciErr sciErr;
    CheckInputArgument(pvApiCtx, 0, 1);

    if (nbInputArgument(pvApiCtx) == 0)
    {
        // nothing
    }
    else
    {
        //get variable address of the input argument
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        getVarType(pvApiCtx, piAddr1, &iType);
        switch (iType)
        {
            case sci_ints:
            {
                /* int */
                int iPrecision = 0;
                int* pi32Data = NULL;
                unsigned int* pui32Data = NULL;

                getMatrixOfIntegerPrecision(pvApiCtx, piAddr1, &iPrecision);
                if (iPrecision != SCI_INT32 && iPrecision != SCI_UINT32)
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A int32 expected.\n"), fname, 1);
                    return 1;
                }

                if (iPrecision == SCI_INT32)
                {
                    sciErr = getMatrixOfInteger32(pvApiCtx, piAddr1, &m1, &n1, &pi32Data);
                    uiVar1 = (unsigned int)pi32Data[0];
                }
                else
                {
                    sciErr = getMatrixOfUnsignedInteger32(pvApiCtx, piAddr1, &m1, &n1, &pui32Data);
                    uiVar1 = pui32Data[0];
                }

                if (sciErr.iErr)
                {
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                    printError(&sciErr, 0);
                    return 1;
                }
                break;
            }
            case sci_matrix:
            {
                /* double */
                double* pdblData = NULL;
                sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &m1, &n1, &pdblData);
                if (sciErr.iErr)
                {
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                    printError(&sciErr, 0);
                    return 1;
                }

                uiVar1 = (unsigned int)pdblData[0];
                break;
            }
            case sci_strings:
            {
                /* string */
                //fisrt call to retrieve dimensions
                sciErr = getMatrixOfString(pvApiCtx, piAddr1, &m1, &n1, NULL, NULL);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 1;
                }

                piLen = (int*)MALLOC(sizeof(int) * m1 * n1);

                //second call to retrieve length of each string
                sciErr = getMatrixOfString(pvApiCtx, piAddr1, &m1, &n1, piLen, NULL);
                if (sciErr.iErr)
                {
                    free(piLen);
                    printError(&sciErr, 0);
                    return 1;
                }

                Str1 = (char**)MALLOC(sizeof(char*) * m1 * n1);
                for (i = 0 ; i < m1 * n1 ; i++)
                {
                    Str1[i] = (char*)MALLOC(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
                }

                //third call to retrieve data
                sciErr = getMatrixOfString(pvApiCtx, piAddr1, &m1, &n1, piLen, Str1);
                if (sciErr.iErr)
                {
                    free(piLen);
                    freeArrayOfString(Str1, m1 * n1);
                    printError(&sciErr, 0);
                    return 1;
                }

                for (j = 0; j < m1 * n1; j++)
                {
                    for (i = 0; i < nb_flag; i++)
                    {
                        if (strcmp(Str1[j], Str[i]) == 0)
                        {
                            break;
                        }
                    }

                    if (i == nb_flag)
                    {
                        free(piLen);
                        freeArrayOfString(Str1, m1 * n1);
                        Scierror(999, _("%s: Wrong values for input argument #%d: FFTW flag expected.\n"), fname, 1);
                        return 0;
                    }
                    else
                    {
                        if (i > 0)
                        {
                            flagv = ( flagv | (1U << (i - 1)) );
                        }
                    }
                }

                uiVar1 = (unsigned int)flagv;
                free(piLen);
                freeArrayOfString(Str1, m1 * n1);
                m1 = 1;
                n1 = 1;
                break;
            }
            default:
                Scierror(53, _("%s: Wrong type for input argument #%d.\n"), fname, 1);
                return 1;
        }

        if (m1 != 1 || n1 != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), fname, 1, 1, 1);
            return 1;
        }

        setCurrentFftwFlags(uiVar1);
    }

    /* return value of Sci_Plan.flags in position 2 */
    sciErr = allocMatrixOfInteger32(pvApiCtx, nbInputArgument(pvApiCtx) + 2, 1, 1, &piDataOut);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 1;
    }

    piDataOut[0] = (int) getCurrentFftwFlags();

    /*Test for only FFTW_MEASURE*/
    if (getCurrentFftwFlags() == 0)
    {
        j = 1;
        if ((Str3 = (char **)MALLOC(sizeof(char *))) == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 1;
        }

        Str3[0] = os_strdup(Str[0]);
        if (Str3[0] == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            FREE(Str3);
            return 1;
        }
    }
    else
    {
        j = 0;
        for (i = 1; i < nb_flag; i++)
        {
            if ((getCurrentFftwFlags() & flagt[i]) == flagt[i])
            {
                j++;
                if (Str3)
                {
                    Str3 = (char **)REALLOC(Str3, sizeof(char *) * j);
                }
                else
                {
                    Str3 = (char **)MALLOC(sizeof(char *) * j);
                }

                if (Str3 == NULL)
                {
                    Scierror(999, _("%s: No more memory.\n"), fname);
                    return 1;
                }

                Str3[j - 1] = os_strdup(Str[i]);
                if (Str3[j - 1] == NULL)
                {
                    Scierror(999, _("%s: No more memory.\n"), fname);
                    freeArrayOfString(Str3, j);
                    return 1;
                }
            }
        }
    }

    if (Str3 == NULL)
    {
        Scierror(999, _("%s: Failed to generate the planner name.\n"), fname);
        return 1;
    }

    /* Create the string matrix as return of the function */
    sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 3, j, 1, Str3);
    freeArrayOfString(Str3, j); // Data have been copied into Scilab memory

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 2;
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 3;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
