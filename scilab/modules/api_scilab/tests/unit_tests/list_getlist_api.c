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

            
int list_getlist(char *fname,unsigned long fname_len)
{
    int iRet        = 0;
    int iItemM      = 0;
    int iItemT      = 0;

    int *piAddr     = NULL;
    int *piAddrM    = NULL;
    int *piAddrT    = NULL;

    CheckRhs(1,1);

    //input data structure
    //list( 
    //  mlist(
    //      a = 1
    //      b = 2
    //      c = 3)
    //  tlist(
    //      value1 = ['a', 'b', 'c']
    //      value2 = [1,2,3]
    //      value3 = int32([1,2,3]))

    iRet = getVarAddressFromPosition(1, &piAddr);
    if(iRet)
    {
        return 1;
    }

    //get MList info
    iRet = getMListInList(1, piAddr, 1, &piAddrM);
    if(iRet)
    {
        return 1;
    }

    iRet = getListItemNumber(piAddrM, &iItemM);
    if(iRet)
    {
        return 1;
    }
    sciprint("MList %d items\n", iItemM);


    //get TList info
    iRet = getTListInList(1, piAddr, 2, &piAddrT);
    if(iRet)
    {
        return 1;
    }

    iRet = getListItemNumber(piAddrT, &iItemT);
    if(iRet)
    {
        return 1;
    }

    sciprint("TList %d items\n", iItemT);

    LhsVar(1) = 0;
    return 0;
}
        
        
