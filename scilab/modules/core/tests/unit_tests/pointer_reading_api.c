/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Scilab Consortium Operational Team
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "api_variable.h"

            
int read_pointer(char *fname,unsigned long fname_len)
{
    int iRet    = 0;

    CheckRhs(0,1);
    CheckLhs(1,1);

    if(Rhs == 0)
    {//create mode
        double* pdblData    = (double*)malloc(sizeof(double) * 2 * 2);
        pdblData[0]         = 1;
        pdblData[1]         = 3;
        pdblData[2]         = 2;
        pdblData[3]         = 4;

        iRet = createPointer(Rhs + 1, (void*)pdblData);
    }
    else
    {//read mode
        int* piAddr         = NULL;
        void* pvPtr         = NULL;
        double* pdblData    = NULL;

        iRet = getVarAddressFromPosition(1, &piAddr);
        if(iRet)
        {
            return 1;
        }

        if(getVarType(piAddr) != sci_lufact_pointer)
        {
            return 1;
        }

        iRet = getPointer(piAddr, &pvPtr);
        if(iRet)
        {
            return 1;
        }

        pdblData = (double *) ((unsigned long int)((double*)pvPtr)[0]);

        iRet = createMatrixOfDouble(Rhs + 1, 2, 2, pdblData);
    }

    if(iRet)
    {
        return 1;
    }

    LhsVar(1) = Rhs + 1;
    return 0;
}
            
        
