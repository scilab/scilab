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

            
int write_sparse(char *fname,unsigned long fname_len)
{
    int iRet                = 0;

    int piNbItemRow[]       = {1,2,1};
    int piColPos[]          = {8,4,7,2};
    double pdblSReal[]      = {1,2,3,4};
    double pdblSImg[]       = {4,3,2,1};
    int iNbItem             = 4;
    
    iRet = createComplexSparseMatrix(Rhs + 1, 3, 10, iNbItem, piNbItemRow, piColPos, pdblSReal, pdblSImg);
    if(iRet)
    {
        return 1;
    }

    LhsVar(1) = 1;
    return 0;
}
        
        
