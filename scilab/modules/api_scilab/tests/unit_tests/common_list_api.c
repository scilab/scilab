/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"

int get_list_info(void* _pvCtx, int* _piAddress);
void insert_indent(void);

static int iLocalTab = 1;

int common_list(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int *piAddr = NULL;

    CheckInputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    get_list_info(pvApiCtx, piAddr);
    AssignOutputVariable(pvApiCtx, 1) = 0;
    return 0;
}
int get_list_info(void* _pvCtx, int* _piAddress)
{
    SciErr sciErr;
    int i       = 0;
    int iRet    = 0;
    int iItem   = 0;

    //get list item number, failed if variable is not a kind of list
    sciErr = getListItemNumber(_pvCtx, _piAddress, &iItem);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        sciprint("This variable is not a list");
        return 0;
    }

    sciprint("List (%d items) -> address : 0x%08X) : \n", iItem, _piAddress);

    for (i = 0 ; i < iItem ; i++)
    {
        int iType           = 0;
        int* piAddrChild    = NULL;

        sciErr = getListItemAddress(_pvCtx, _piAddress, i + 1, &piAddrChild);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        sciErr = getVarType(_pvCtx, piAddrChild, &iType);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        if (iType == sci_list || iType == sci_tlist || iType == sci_mlist)
        {
            insert_indent();
            sciprint("Child %d -> ", i + 1);
            iLocalTab++;
            iRet = get_list_info(_pvCtx, piAddrChild);
            iLocalTab--;

            if (iRet)
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
    for (i = 0 ; i < iLocalTab ; i++)
    {
        sciprint("\t");
    }
}
