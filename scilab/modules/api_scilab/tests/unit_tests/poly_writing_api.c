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

            
int write_poly(char *fname,unsigned long fname_len)
{
    int iRet                = 0;

    //output variable info : polinomial matrix 2 x 4
    //[ x + 2                   x^2 - 4x + 5    4x^3 - 14x^2 + 18 ;
    //  2x^3 - 12x^2 + 64       1               8x^5 + 32x^3]
    int iRows               = 2;
    int iCols               = 3;

    //varname "x"
    char pstVarName[2]      = {"x"};

    //coeficient array
    int piNbCoef[6]         = {2,4,3,1,4,6};

    //data array
    double *pdblReal[6]     = {0};
    double pdblPoly0[2]     = {2,1};
    double pdblPoly1[4]     = {64,0,-12,2};
    double pdblPoly2[3]     = {5,-4,1};
    double pdblPoly3[1]     = {1};
    double pdblPoly4[4]     = {18,0,-14,4};
    double pdblPoly5[6]     = {0,0,0,32,0,8};

    pdblReal[0]             = pdblPoly0;
    pdblReal[1]             = pdblPoly1;
    pdblReal[2]             = pdblPoly2;
    pdblReal[3]             = pdblPoly3;
    pdblReal[4]             = pdblPoly4;
    pdblReal[5]             = pdblPoly5;

    createMatrixOfPoly(Rhs + 1, pstVarName, iRows, iCols, piNbCoef, pdblReal);

    //assign allocated variables to Lhs position
    LhsVar(1) = Rhs + 1;
    return 0;
}
        
        
