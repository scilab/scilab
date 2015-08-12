/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "MALLOC.h"

int write_boolean(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int i, j;
    //first variable info : boolean matrix of boolean 3 x 4
    int iRows1			= 3;
    int iCols1			= 4;
    int* pboolOutput1	= NULL;
    //second variable info : boolean matrix of boolean 4 x 6
    int iRows2			= 4;
    int iCols2			= 6;
    int* pboolOutput2	= NULL;

    /************************
    *    First variable    *
    ************************/

    //alloc array of data in OS memory
    pboolOutput1 = (int*)malloc(sizeof(int) * iRows1 * iCols1);
    //fill array with incremental values
    //[ %t  %f  %t  %f
    //  %t  %f  %t  %f
    //  %t  %f  %t  %f ]
    for (i = 0 ; i < iRows1 ; i++)
    {
        for (j = 0 ; j < iCols1 ; j++)
        {
            pboolOutput1[i + iRows1 * j] = (i * iCols1 + j) % 2 == FALSE;
        }
    }

    //can be written in a single loop
    //for(i = 0 ; i < iRows1 * iCols1; i++)
    //{
    //  pboolOutput1[i] = i % 2 == FALSE;
    //}
    //create a variable from a existing data array

    sciErr = createMatrixOfBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iRows1, iCols1, pboolOutput1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    //after creation, we can free memory.
    free(pboolOutput1);


    /*************************
    *    Second variable    *
    *************************/

    //reserve space in scilab memory and fill it
    sciErr = allocMatrixOfBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 2, iRows2, iCols2, &pboolOutput2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //fill array with incremental values
    //[ %f  %t  %f  %t  %f  %t
    //  %f  %t  %f  %t  %f  %t
    //  %f  %t  %f  %t  %f  %t
    //  %f  %t  %f  %t  %f  %t ]
    for (i = 0 ; i < iRows2 ; i++)
    {
        for (j = 0 ; j < iCols2 ; j++)
        {
            pboolOutput2[i + iRows2 * j] = (i * iCols2 + j) % 2 == TRUE;
        }
    }

    //can be written in a single loop
    //for(i = 0 ; i < iRows2 * iCols2; i++)
    //{
    //  pboolOutput2[i] = i % 2 == TRUE;
    //}
    // /!\ DO NOT FREE MEMORY, in this case, it's the Scilab memory
    //assign allocated variables to Lhs position

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;

    return 0;
}
