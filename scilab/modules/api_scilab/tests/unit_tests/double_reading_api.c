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

            
int read_double(char *fname,unsigned long fname_len)
{
    int i;
    int iRet            = 0;
    //first variable info : real matrix of double
    int iRows           = 0;
    int iCols           = 0;
    int iComplex        = 0;
    int *piAddr         = NULL;
    double* pdblReal    = NULL;
    double* pdblImg     = NULL;

    //check input and output arguments
    CheckRhs(1,1);
    CheckLhs(1,1);

    /************************
    *    First variable    *
    ************************/

    //get variable address of the first input argument
    getVarAddressFromPosition(1, &piAddr);

    //check type
    if(getVarType(piAddr) != sci_matrix)
    {
        //Error
        return 1;
    }
    
    //get complexity
    iComplex	= isVarComplex(piAddr);

    //check complexity
    if(iComplex)
    {
        //get size and data from Scilab memory
        iRet = getComplexMatrixOfDouble(piAddr, &iRows, &iCols, &pdblReal, &pdblImg);
    }
    else
    {
        //get size and data from Scilab memory
        iRet = getMatrixOfDouble(piAddr, &iRows, &iCols, &pdblReal);
    }

    if(iRet)
    {
        //Error
        return 1;
    }

    //Do something with data
    //if variable is complex, switch real part and imaginary part otherwise multiply by -1

    if(iComplex)
    {
        createComplexMatrixOfDouble(Rhs + 1, iRows, iCols, pdblImg, pdblReal);
    }
    else
    {
        for(i = 0 ; i < iRows * iCols ; i++)
        {
            pdblReal[i] = pdblReal[i] * -1;
        }
        createMatrixOfDouble(Rhs + 1, iRows, iCols, pdblReal);
    }

    if(iRet)
    {
        //Error
        return 1;
    }
    LhsVar(1) = Rhs + 1;
    return 0;
}
        
        
