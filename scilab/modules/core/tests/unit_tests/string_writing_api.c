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

            
int write_string(char *fname,unsigned long fname_len)
{
    int iRet        = 0;

    //variable info : matrix of string 2 x 3
    int iRows       = 2;
    int iCols       = 3;
    char** pstData  = NULL;

    //data to put in the new variable
    char string0[]  = "may";
    char string1[]  = "be";
    char string2[]  = "the";
    char string3[]  = "with";
    char string4[]  = "puffin";
    char string5[]  = "you";

    //alloc new array
    pstData         = (char**)malloc(sizeof(char*) * iRows * iCols);

    //copy data address to the "main" array
    pstData[0]      = string0;
    pstData[1]      = string1;
    pstData[2]      = string2;
    pstData[3]      = string3;
    pstData[4]      = string4;
    pstData[5]      = string5;

    //create the variable
    createMatrixOfString(Rhs + 1, iRows, iCols, pstData);

    //free container
    free(pstData);
    
    //assign allocated variables to Lhs position
    LhsVar(1)       = Rhs + 1;
    return 0;
}
        
        
