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

            
int write_double(char *fname,unsigned long fname_len)
{
    int i,j;
    int iRet            = 0;
    //first variable info : real matrix of double 3 x 4
    int iRows1          = 3;
    int iCols1          = 4;
    double* pdblReal1   = NULL;

    //second variable info : complex matrix of double 4 x 6
    int iRows2          = 4;
    int iCols2          = 6;
    double* pdblReal2   = NULL;
    double* pdblImg2    = NULL;

    /************************
    *    First variable    *
    ************************/
    //alloc array of data in OS memory
    pdblReal1 = (double*)malloc(sizeof(double) * iRows1 * iCols1);

    //fill array with incremental values
    //[ 0   1   2   3
    //  4   5   6   7
    //  8   9   10  11]
    for(i = 0 ; i < iRows1 ; i++)
    {
        for(j = 0 ; j < iCols1 ; j++)
        {
            pdblReal1[i + iRows1 * j] = i * iCols1 + j;
        }
    }
    //can be written in a single loop
    //for(i = 0 ; i < iRows1 * iCols1; i++)
    //{
    //  pdblReal1[i] = i;
    //}

    //create a variable from a existing data array
    iRet = createMatrixOfDouble(Rhs + 1, iRows1, iCols1, pdblReal1);
    if(iRet)
    {
        //Error
        return 1;
    }

    //after creation, we can free memory.
    free(pdblReal1);
    /*************************
    *    Second variable    *
    *************************/

    //reserve space in scilab memory and fill it
    iRet = allocComplexMatrixOfDouble(Rhs + 2, iRows2, iCols2, &pdblReal2, &pdblImg2);
    if(iRet)
    {
        //Error
        return 1;
    }

    //fill array with incremental values for real part and decremental for imaginary part
    //[ 23i     1+22i       2+21i       3+20i       4+19i       5+18i
    //  6+17i   7+16i       8+15i       9+14i       10+13i      11+12i
    //  12+11i  13+10i      14+9i       15+8i       16+7i       17+6i
    //  18+5i   19+4i       20+3i       21+2i       22+1i       23  ]
    for(i = 0 ; i < iRows2 ; i++)
    {
       for(j = 0 ; j < iCols2 ; j++)
        {
            pdblReal2[i + iRows2 * j] = i * iCols2 + j;
            pdblImg2 [i + iRows2 * j]	= (iRows2 * iCols2 - 1) - (i * iCols2 + j);
        }
    }
    //can be written in a single loop
    //for(i = 0 ; i < iRows2 * iCols2; i++)
    //{
    //  pdblReal2[i] = i;
    //  pdblImg2 [i] = (iRows2 * iCols2 - 1) - i;
    //}

    // /!\ DO NOT FREE MEMORY, in this case, it's the Scilab memory


    //assign allocated variables to Lhs position
    LhsVar(1) = Rhs + 1;
    LhsVar(2) = Rhs + 2;
    return 0;
}
        
        
