/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Scilab Consortium Operational Team
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "api_scilab.h"
#include "MALLOC.h"

int get_list_info(int* _piAddress);
void insert_indent(void);
static int iLocalTab = 0;
int common_list(char *fname,unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddr     = NULL;
    CheckRhs(1,1);
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    get_list_info(piAddr);
    LhsVar(1) = 0;
    return 0;
}
int get_list_info(int* _piAddress)
{
    SciErr sciErr;
    int i       = 0;
    int iRet    = 0;
    int iItem   = 0;
    //get list item number, failed if variable is not a kind of list
    sciErr = getListItemNumber(pvApiCtx, _piAddress, &iItem);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        sciprint("This variable is not a list");
        return 0;
    }
    sciprint("List (%d items) -> address : 0x%08X) : \n", iItem, _piAddress);
    for(i = 0 ; i < iItem ; i++)
    {
        int iType           = 0;
        int* piAddrChild    = NULL;
        sciErr = getListItemAddress(pvApiCtx, _piAddress, i + 1, &piAddrChild);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        sciErr = getVarType(pvApiCtx, piAddrChild, &iType);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
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
