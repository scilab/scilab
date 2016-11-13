/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO
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
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "sci_malloc.h"

int read_string(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int i, j;
    int iLen		= 0;
    //variable info
    int iRows		= 0;
    int iCols		= 0;
    int* piAddr		= NULL;
    int* piLen		= NULL;
    char** pstData	= NULL;
    //output variable
    int iRowsOut	= 1;
    int iColsOut	= 1;
    char* pstOut	= NULL;
    //check input and output arguments

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //first call to retrieve dimensions
    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, NULL, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    piLen = (int*)MALLOC(sizeof(int) * iRows * iCols);

    //second call to retrieve length of each string
    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    pstData = (char**)MALLOC(sizeof(char*) * iRows * iCols);
    for (i = 0 ; i < iRows * iCols ; i++)
    {
        pstData[i] = (char*)MALLOC(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
    }

    //third call to retrieve data
    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, pstData);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //computer length of all strings
    for (i = 0 ; i < iRows * iCols ; i++)
    {
        iLen += piLen[i];
    }

    //alloc output variable
    pstOut = (char*)MALLOC(sizeof(char) * (iLen + iRows * iCols));
    //initialize string to 0x00
    memset(pstOut, 0x00, sizeof(char) * (iLen + iRows * iCols));

    //concat input strings in output string
    for (i = 0 ; i < iRows ; i++)
    {
        for (j = 0 ; j < iCols ; j++)
        {
            int iCurLen = strlen(pstOut);
            if (iCurLen)
            {
                strcat(pstOut, " ");
            }
            strcpy(pstOut + strlen(pstOut), pstData[j * iRows + i]);
        }
    }

    //create new variable
    sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iRowsOut, iColsOut, &pstOut);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        //free memory
        FREE(piLen);
        for (i = 0 ; i < iRows * iCols ; i++)
        {
            FREE(pstData[i]);
        }
        FREE(pstData);
        FREE(pstOut);
        return 0;
    }

    //free memory
    FREE(piLen);

    for (i = 0 ; i < iRows * iCols ; i++)
    {
        FREE(pstData[i]);
    }

    FREE(pstData);
    FREE(pstOut);
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    return 0;
}
