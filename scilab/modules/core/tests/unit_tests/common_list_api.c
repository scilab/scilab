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

            
int get_list_info(int* _piAddress);
void insert_indent(void);

int iLocalTab = 0;

int common_list(char *fname,unsigned long fname_len)
{
    int iRet        = 0;
    int iItem       = 0;

    int *piAddr     = NULL;

    CheckRhs(1,1);


    iRet = getVarAddressFromPosition(1, &piAddr);
    if(iRet)
    {
        return 1;
    }

    iRet = get_list_info(piAddr);
    if(iRet)
    {
        return 1;
    }

    LhsVar(1) = 0;
    return 0;
}

int get_list_info(int* _piAddress)
{
    int i       = 0;
    int iRet    = 0;
    int iItem   = 0;
    
    //get list item number, failed if variable is not a kind of list
    iRet = getListItemNumber(_piAddress, &iItem);
    if(iRet)
    {
        sciprint("This variable is not a list");
        return 1;
    }

    sciprint("List (%d items) -> address : 0x%08X) : \n", iItem, _piAddress);
    for(i = 0 ; i < iItem ; i++)
    {
        int iType           = 0;
        int* piAddrChild    = NULL;
        iRet = getListItemAddress(_piAddress, i + 1, &piAddrChild);
        iType = getVarType(piAddrChild);

        if(iType == sci_list || iType == sci_tlist || iType == sci_mlist)
        {
            insert_indent();
            sciprint("Child %d -> ", i + 1);
            iLocalTab++;
            iRet = get_list_info(piAddrChild);
            iLocalTab--;
            if(iRet)
            {
              return 1;
            }
        }
        else
        {
            insert_indent();
            sciprint("Child %d -> address : 0x%08X\n", i + 1, piAddrChild);
        }
    }
    return 0;
}

void insert_indent(void)
{
    int i = 0;
    for(i = 0 ; i < iLocalTab ; i++)
    {
        sciprint("\t");
    }
}
        
        
