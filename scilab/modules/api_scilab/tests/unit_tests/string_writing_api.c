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

int write_string(char *fname, void* pvApiCtx)
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
    pstData			= (char**)MALLOC(sizeof(char*) * iRows * iCols);
    //copy data address to the "main" array
    pstData[0]		= string11;
    pstData[1]		= string21;
    pstData[2]		= string12;
    pstData[3]		= string22;
    pstData[4]		= string13;
    pstData[5]		= string23;

    //create the variable
    sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iRows, iCols, pstData);
    //free container
    FREE(pstData);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //assign allocated variables to Lhs position
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    return 0;
}
