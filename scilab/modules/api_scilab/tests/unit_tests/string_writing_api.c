/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "MALLOC.h"

int write_string(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    //variable info : matrix of string 2 x 3
    int iRows		= 2;
    int iCols		= 3;
    char** pstData	= NULL;
    //data to put in the new variable
    char string11[]	= "may";
    char string21[]	= "be";
    char string12[]	= "the";
    char string22[]	= "with";
    char string13[]	= "puffin";
    char string23[]	= "you";
    //alloc new array
    pstData			= (char**)malloc(sizeof(char*) * iRows * iCols);
    //copy data address to the "main" array
    pstData[0]		= string11;
    pstData[1]		= string21;
    pstData[2]		= string12;
    pstData[3]		= string22;
    pstData[4]		= string13;
    pstData[5]		= string23;

    //create the variable
    sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iRows, iCols, pstData);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //free container
    free(pstData);
    //assign allocated variables to Lhs position
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    return 0;
}
