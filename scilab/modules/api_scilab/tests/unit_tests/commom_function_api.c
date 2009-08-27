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

            
int printf_info(int _iVar);

int commom_function(char *fname,unsigned long fname_len)
{
    int i;
    int iRet        = 0;
    int *piAddr1    = NULL;

    for(i = 0 ; i < Rhs ; i++)
    {
        iRet = printf_info(i + 1);
        if(iRet)
        {
            break;
        }
        sciprint("\n\n");
    }

    if(iRet)
    {
        //1 for true, 0 for false
        iRet = !iRet;
        createMatrixOfBoolean(1, 1, 1, &iRet);
        //assign allocated variables to Lhs position
        LhsVar(1) = 1;
    }
    else
    {
        //assign allocated variables to Lhs position
        LhsVar(1) = 0;
    }

    return 0;
}

int printf_info(int _iVar)
{
    int* piAddr     = NULL;
    int iRet        = 0;
    int iType       = 0;
    int iRows       = 0;
    int iCols       = 0;
    int iItem       = 0;
    int iComplex    = 0;

    iRet = getVarAddressFromPosition(_iVar, &piAddr);
    if(iRet)
    {
        return 1;
    }

    sciprint("Variable %d information:\n", _iVar);
    iType = getVarType(piAddr);
    sciprint("\tType: ");
    switch(iType)
    {
        case sci_matrix : 
            sciprint("double\n");
            break;
        case sci_poly : 
            sciprint("polynomial\n");
            break;
        case sci_boolean : 
           sciprint("boolean\n");
           break;
        case sci_sparse : 
            sciprint("sparse\n");
            break;
        case sci_boolean_sparse : 
            sciprint("boolean_sparse\n");
            break;
        case sci_ints : 
        {
            char pstSigned[]    = "signed";
            char pstUnsigned[]  = "unsigned";
            char* pstSign       = pstSigned;

            int iPrec           = 0;
            iRet = getMatrixOfIntegerPrecision(piAddr, &iPrec);

            if(iPrec > 10)
            {
                pstSign = pstUnsigned;
            }

            sciprint("%s integer %d bits\n", pstSign, (iPrec % 10) * 8);
        }
        break;
        case sci_strings : 
            sciprint("strings\n");
            break;
        case sci_list : 
            sciprint("list\n");
            break;
        case sci_tlist : 
            sciprint("tlist\n");
            break;
        case sci_mlist : 
            sciprint("mlist\n");
            break;
        default :
            sciprint("Not manage by this function\n");
            return 0;
    }

    if(isVarComplex(piAddr))
    {
        sciprint("\tComplex: Yes\n");
    }

    sciprint("\tDimensions: ");
    if(isVarMatrixType(piAddr))
    {
        getVarDimension(piAddr, &iRows, &iCols);
        sciprint("%d x %d", iRows, iCols);
    }
    else
    {
        getListItemNumber(piAddr, &iItem);
        sciprint("%d", iItem);
    }
    return 0;
}
        
        
