/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/

extern "C"
{
#include <string.h>
#include "MALLOC.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
}

#include "displaytree.hxx"

bool bParseListItem(void* _pvCtx, int *_piAddrTree, int _iItemCount, std::vector<std::string> *_pvStructList, std::string _szLevel)
{
    SciErr sciErr;

    int* piAddrTreeI        = NULL;
    int* piAddrTreeStr      = NULL;
    int* piAddrTreeMList    = NULL;
    int* piAddrLabel        = NULL;
    int* piAddrIcon         = NULL;
    int* piAddrCallback     = NULL;

    char* strItem1      = NULL;
    char* strLabel      = NULL;
    char* strIcon       = NULL;
    char* strCallback   = NULL;

    int iItemCount = 0;

    std::string szCurLvl = "";

    //parse item
    for (int i = 3; i < _iItemCount + 1; i++) //tlist
    {
        sciErr = getListItemAddress(_pvCtx, _piAddrTree, i, &piAddrTreeI);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        if (isTListType(_pvCtx, piAddrTreeI) == FALSE) //potential tree
        {
            //go up, it is finish for this node
            return 0;
        }

        /*retrieve next item*/
        sciErr = getListItemNumber(_pvCtx, piAddrTreeI, &iItemCount);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        if (iItemCount < 2)
        {
            return 1;
        }

        szCurLvl = _szLevel;

        // get first element as a string
        sciErr = getListItemAddress(_pvCtx, piAddrTreeI, 1, &piAddrTreeStr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        if (getAllocatedSingleString(_pvCtx, piAddrTreeStr, &strItem1))
        {
            return 1;
        }

        /*check tree structure */
        if (strcmp(strItem1, TREE_REF_NAME))
        {
            freeAllocatedSingleString(strItem1);
            return 1;
        }
        freeAllocatedSingleString(strItem1);

        sciErr = getListItemAddress(_pvCtx, piAddrTreeI, 2, &piAddrTreeMList);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        if (isMListType(_pvCtx, piAddrTreeMList) == false)
        {
            return 1;
        }

        //Add node level
        szCurLvl += ".";
        std::ostringstream out;
        out << i - 1;
        szCurLvl += out.str();

        _pvStructList->push_back(szCurLvl);

        sciErr = getListItemAddress(_pvCtx, piAddrTreeMList, 1, &piAddrLabel);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }
        int iType = 0;
        getVarType(_pvCtx, piAddrTreeMList, &iType);

        //get label name
        sciErr = getListItemAddress(_pvCtx, piAddrTreeMList, 3, &piAddrLabel);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        if (getAllocatedSingleString(_pvCtx, piAddrLabel, &strLabel))
        {
            return 1;
        }

        _pvStructList->push_back(strLabel);
        freeAllocatedSingleString(strLabel);

        //get Icon name
        sciErr = getListItemAddress(_pvCtx, piAddrTreeMList, 4, &piAddrIcon);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        if (getAllocatedSingleString(_pvCtx, piAddrIcon, &strIcon))
        {
            return 1;
        }

        _pvStructList->push_back(strIcon);
        freeAllocatedSingleString(strIcon);

        //get callback name
        sciErr = getListItemAddress(_pvCtx, piAddrTreeMList, 5, &piAddrCallback);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        if (getAllocatedSingleString(_pvCtx, piAddrCallback, &strCallback))
        {
            return 1;
        }

        _pvStructList->push_back(strCallback);
        freeAllocatedSingleString(strCallback);

        bParseListItem(_pvCtx, piAddrTreeI, iItemCount, _pvStructList, szCurLvl);
    }
    return 0;
}
