/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA
* Copyright (C) DIGITEO - 2010-2011 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#include "gw_output_stream.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "do_xxprintf.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "strsubst.h"
/*--------------------------------------------------------------------------*/
#define PERCENT_CHAR '%'
#define SPLIT_ON_CR_IN_FORMAT "<CR_IN_FORMAT>"
#define CR_IN_FORMAT "\\n"
#define EMPTY_CHAR '\0'
/*--------------------------------------------------------------------------*/
int sci_msprintf(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int iType = 0;
    int *piAddressVarOne = NULL;
    char *ptrFormat = NULL;
    int K = 0;
    int i = 0;
    int lengthFormat = 0;
    int NumberPercent = 0;
    int NumberCols = 0;
    int nmax = 0;
    int cat_to_last = 0;
    int ll = 0;
    char **pStrs = NULL;
    char **pOutputStrings = NULL;
    char *pStrTmp = NULL;
    char *pStrTmp1 = NULL;
    int lcount = 0;
    int rval = 0;
    int blk = 200;

    int k = 0;
    int mOut = 0;
    int nOut = 0;
    int lengthSplitChar = (int)strlen(SPLIT_ON_CR_IN_FORMAT);

    Nbvars = 0;
    CheckRhs(1, 1000);
    CheckLhs(0, 1);

    for (K = 2; K <= Rhs; K++)
    {
        int iTypeK = 0;
        int *piAddressVarK = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, K, &piAddressVarK);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        sciErr = getVarType(pvApiCtx, piAddressVarK, &iTypeK);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if ( (iTypeK != sci_matrix) && (iTypeK != sci_strings) )
        {
            OverLoad(K);
            return 0;
        }
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (checkVarDimension(pvApiCtx, piAddressVarOne, 1, 1) != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &ptrFormat))
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    if (ptrFormat == NULL)
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }
    else
    {
        char *ptrFormatTmp = strsub(ptrFormat, CR_IN_FORMAT, SPLIT_ON_CR_IN_FORMAT);
        if (ptrFormatTmp)
        {
            freeAllocatedSingleString(ptrFormat);
            ptrFormat = ptrFormatTmp;
        }
        else
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }
    }

    lengthFormat = (int)strlen(ptrFormat);
    for (i = 0; i < lengthFormat; i++)
    {
        if (ptrFormat[i] == PERCENT_CHAR)
        {
            NumberPercent++;
            if ( (i + 1 < lengthFormat) && (ptrFormat[i + 1] == PERCENT_CHAR))
            {
                NumberPercent--;
                i++;
            }
        }
    }

    if ( (Rhs - 1) > NumberPercent )
    {
        if (ptrFormat)
        {
            FREE(ptrFormat);
            ptrFormat = NULL;
        }

        Scierror(999, _("%s: Wrong number of input arguments: at most %d expected.\n"), fname, NumberPercent);
        return 0;
    }

    if ( Rhs > 1 )
    {
        for ( i = 2 ; i <= Rhs ; i++ )
        {
            int iRows = 0;
            int iCols = 0;
            int *piAddressVarI = NULL;

            sciErr = getVarAddressFromPosition(pvApiCtx, i, &piAddressVarI);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, i);
                return 0;
            }

            sciErr = getVarDimension(pvApiCtx, piAddressVarI, &iRows, &iCols);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, i);
                return 0;
            }
            NumberCols += iCols;
        }
    }

    if ( NumberCols != NumberPercent )
    {
        if (ptrFormat)
        {
            FREE(ptrFormat);
            ptrFormat = NULL;
        }
        Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), fname);
        return 0;
    }

    mOut = 0; /* output line counter */
    nmax = 0;
    pOutputStrings = NULL;
    lcount = 1;
    cat_to_last = 0;

    while (1)
    {
        if ((rval = do_xxprintf("msprintf", (FILE *) 0, ptrFormat, Rhs, 1, lcount, (char **) &pStrs)) < 0)
        {
            break;
        }

        lcount++;
        pStrTmp = (char *)pStrs;
        if (pStrTmp == NULL)
        {
            if (ptrFormat)
            {
                FREE(ptrFormat);
                ptrFormat = NULL;
            }

            Scierror(999, _("%s: Wrong value of input argument #%d: data doesn't fit with format.\n"), fname, 1);
            return 0;
        }

        pStrTmp1 = pStrTmp;
        while (*pStrTmp != '\0')
        {
            if (strncmp(pStrTmp, SPLIT_ON_CR_IN_FORMAT, lengthSplitChar) == 0)
            {
                k = (int)(pStrTmp - pStrTmp1);
                if (!cat_to_last)
                {
                    /*add a new line */
                    if (mOut == nmax)
                    {
                        nmax += blk;
                        if (pOutputStrings)
                        {
                            pOutputStrings = (char **) REALLOC(pOutputStrings, nmax * sizeof(char **));
                        }
                        else
                        {
                            pOutputStrings = (char **) MALLOC(nmax * sizeof(char **));
                        }

                        if (pOutputStrings == NULL)
                        {
                            if (ptrFormat)
                            {
                                FREE(ptrFormat);
                                ptrFormat = NULL;
                            }

                            Scierror(999, _("%s: No more memory.\n"), fname);
                            return 0;
                        }
                    }

                    pOutputStrings[mOut] = (char*)MALLOC((k + 1) * sizeof(char));
                    if (pOutputStrings[mOut] == NULL)
                    {
                        if (ptrFormat)
                        {
                            FREE(ptrFormat);
                            ptrFormat = NULL;
                        }
                        freeArrayOfString(pOutputStrings, mOut * nOut);
                        Scierror(999, _("%s: No more memory.\n"), fname);
                        return 0;
                    }
                    strncpy(pOutputStrings[mOut], pStrTmp1, k);
                    pOutputStrings[mOut][k] = EMPTY_CHAR;
                    mOut++;
                }
                else
                {
                    /* cat to previous line */
                    ll = (int)strlen(pOutputStrings[mOut - 1]);
                    pOutputStrings[mOut - 1] = (char*)REALLOC(pOutputStrings[mOut - 1], (k + 1 + ll) * sizeof(char));
                    if (pOutputStrings[mOut - 1] == NULL)
                    {
                        if (ptrFormat)
                        {
                            FREE(ptrFormat);
                            ptrFormat = NULL;
                        }

                        Scierror(999, _("%s: No more memory.\n"), fname);
                        return 0;
                    }
                    strncpy(&(pOutputStrings[mOut - 1][ll]), pStrTmp1, k);
                    pOutputStrings[mOut - 1][k + ll] = EMPTY_CHAR;
                }
                pStrTmp += lengthSplitChar;
                pStrTmp1 = pStrTmp;
                cat_to_last = 0;
            }
            else
            {
                pStrTmp++;
            }
        }
        k = (int)(pStrTmp - pStrTmp1);
        if (k > 0)
        {
            if ((!cat_to_last) || (mOut == 0))
            {
                /*add a new line */
                if (mOut == nmax)
                {
                    nmax += blk;
                    if (pOutputStrings)
                    {
                        pOutputStrings = (char **) REALLOC(pOutputStrings, nmax * sizeof(char **));
                        if (pOutputStrings == NULL)
                        {
                            if (ptrFormat)
                            {
                                FREE(ptrFormat);
                                ptrFormat = NULL;
                            }

                            Scierror(999, _("%s: No more memory.\n"), fname);
                            return 0;
                        }
                    }
                    else
                    {
                        pOutputStrings = (char **) MALLOC(nmax * sizeof(char **));
                        if (pOutputStrings == NULL)
                        {
                            if (ptrFormat)
                            {
                                FREE(ptrFormat);
                                ptrFormat = NULL;
                            }

                            Scierror(999, _("%s: No more memory.\n"), fname);
                            return 0;
                        }
                    }
                }

                pOutputStrings[mOut] = (char*) MALLOC((k + 1) * sizeof(char));
                if (pOutputStrings[mOut] == NULL)
                {
                    if (ptrFormat)
                    {
                        FREE(ptrFormat);
                        ptrFormat = NULL;
                    }
                    freeArrayOfString(pOutputStrings, mOut * nOut);
                    Scierror(999, _("%s: No more memory.\n"), fname);
                    return 0;
                }
                strncpy(pOutputStrings[mOut], pStrTmp1, k);
                pOutputStrings[mOut][k] = EMPTY_CHAR;
                mOut++;
            }
            else
            {
                /* cat to previous line */
                ll = (int)strlen(pOutputStrings[mOut - 1]);
                pOutputStrings[mOut - 1] = (char*)REALLOC(pOutputStrings[mOut - 1], (k + 1 + ll) * sizeof(char));
                if (pOutputStrings[mOut - 1] == NULL)
                {
                    if (ptrFormat)
                    {
                        FREE(ptrFormat);
                        ptrFormat = NULL;
                    }

                    Scierror(999, _("%s: No more memory.\n"), fname);
                    return 0;
                }
                strncpy(&(pOutputStrings[mOut - 1][ll]), pStrTmp1, k);
                pOutputStrings[mOut - 1][k + ll] = EMPTY_CHAR;
            }
        }

        if (strncmp(pStrTmp - lengthSplitChar, SPLIT_ON_CR_IN_FORMAT, lengthSplitChar) != 0)
        {
            cat_to_last = 1;
        }
        if (Rhs == 1)
        {
            break;
        }

    }

    if (ptrFormat)
    {
        FREE(ptrFormat);
        ptrFormat = NULL;
    }

    if (rval == RET_BUG)
    {
        return 0;
    }

    /* Create a Scilab String */
    nOut = 1;
    sciErr = createMatrixOfString(pvApiCtx, Rhs + 1 , mOut, nOut, pOutputStrings);

    /* lstr must not be freed */
    freeArrayOfString(pOutputStrings, mOut * nOut);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
    }
    else
    {
        LhsVar(1) = Rhs + 1;
        PutLhsVar();
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
