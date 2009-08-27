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

            
int read_poly(char *fname,unsigned long fname_len)
{
    int i,j;
    int iRet            = 0;

    //variable info
    int iRows           = 0;
    int iCols           = 0;
    int iVarLen         = 0;
    int* piAddr         = NULL;
    int* piNbCoef       = NULL;
    double** pdblReal   = NULL;
    double** pdblImg    = NULL;
    char* pstVarname    = NULL;

    //check input and output arguments
    CheckRhs(1,1);
    CheckLhs(1,1);

    iRet = getVarAddressFromPosition(1, &piAddr);
    if(iRet)
    {
        return 1;
    }

    if(getVarType(piAddr) != sci_poly || isVarComplex(piAddr) == FALSE)
    {
        return 1;
    }

    //get variable name length
    iRet = getPolyVariableName(piAddr, NULL, &iVarLen);
    if(iRet)
    {
        return 1;
    }

    //alloc buff to receive variable name
    pstVarname = (char*)malloc(sizeof(char) * (iVarLen + 1));//1 for null termination
    //get variable name
    iRet = getPolyVariableName(piAddr, pstVarname, &iVarLen);
    if(iRet)
    {
        return 1;
    }

    //First call: retrieve dimmension
    iRet = getComplexMatrixOfPoly(piAddr, &iRows, &iCols, NULL, NULL, NULL);
    if(iRet)
    {
        return 1;
    }

    //alloc array of coefficient
    piNbCoef = (int*)malloc(sizeof(int) * iRows * iCols);
    //Second call: retrieve coefficient
    iRet = getComplexMatrixOfPoly(piAddr, &iRows, &iCols, piNbCoef, NULL, NULL);
    if(iRet)
    {
        return 1;
    }

    //alloc arrays of data
    pdblReal    = (double**)malloc(sizeof(double*) * iRows * iCols);
    pdblImg     = (double**)malloc(sizeof(double*) * iRows * iCols);
    for(i = 0 ; i < iRows * iCols ; i++)
    {
        pdblReal[i] = (double*)malloc(sizeof(double) * piNbCoef[i]);
        pdblImg[i] = (double*)malloc(sizeof(double) * piNbCoef[i]);
    }

    //Third call: retrieve data
    iRet = getComplexMatrixOfPoly(piAddr, &iRows, &iCols, piNbCoef, pdblReal, pdblImg);
    if(iRet)
    {
        return 1;
    }


    //Do something with Data

    //Invert polynomials in the matrix and invert coefficients

    for(i = 0 ; i < (iRows * iCols) / 2 ; i++)
    {
        int iPos1           = iRows * iCols - 1 - i;
        double* pdblSave    = NULL;
        int iNbCoefSave     = 0;

        //switch array of coefficient
        pdblSave            = pdblReal[i];
        pdblReal[i]         = pdblReal[iPos1];
        pdblReal[iPos1]     = pdblSave;

        pdblSave            = pdblImg[i];
        pdblImg[i]          = pdblImg[iPos1];
        pdblImg[iPos1]      = pdblSave;

        //switch number of coefficient
        iNbCoefSave         = piNbCoef[i];
        piNbCoef[i]         = piNbCoef[iPos1];
        piNbCoef[iPos1]     = iNbCoefSave;
    }

    //switch coefficient
    for(i = 0 ; i < iRows * iCols ; i++)
    {
        for(j = 0 ; j < piNbCoef[i] /2 ; j++)
        {
            int iPos2           = piNbCoef[i] - 1 - j;
            double dblVal       = pdblReal[i][j];
            pdblReal[i][j]      = pdblReal[i][iPos2];
            pdblReal[i][iPos2]  = dblVal;

            dblVal              = pdblImg[i][j];
            pdblImg[i][j]       = pdblImg[i][iPos2];
            pdblImg[i][iPos2]   = dblVal;
        }
    }
    
    createComplexMatrixOfPoly(Rhs + 1, pstVarname, iRows, iCols, piNbCoef, pdblReal, pdblImg);
 
    //free OS memory
    free(pstVarname);
    free(piNbCoef);

    for(i = 0 ; i < iRows * iCols ; i++)
    {
        free(pdblReal[i]);
        free(pdblImg[i]);
    }

    free(pdblReal);
    free(pdblImg);

    //assign allocated variables to Lhs position
    LhsVar(1) = Rhs + 1;
    return 0;
}
            
        
