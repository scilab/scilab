/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/

#include <vector>
#include <sstream>
#include <iostream>
#include <string>

#include "displaytree.hxx"
#include "ScilabDisplayTree.hxx"
#include "GiwsException.hxx"

extern "C"
{
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif

#include "api_scilab.h"
#include "gw_gui.h"
#include "stdlib.h"
#include "sciprint.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "getScilabJavaVM.h"
}

/*--------------------------------------------------------------------------*/
int sci_displaytree(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    int* piAddrTree         = NULL;
    int* piAddrTreeStr      = NULL;
    int* piAddrTreeMList    = NULL;
    int* piAddrLabel        = NULL;
    int* piAddrIcon         = NULL;
    int* piAddrCallback     = NULL;

    int iItemCount = 0;

    char* strItem1      = NULL;
    char* strLabel      = NULL;
    char* strIcon       = NULL;
    char* strCallback   = NULL;

    vector<std::string> StructList;
    std::string szCurLevel = "";

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrTree);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getListItemNumber(pvApiCtx, piAddrTree, &iItemCount);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (iItemCount < 2)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: At least 2 elements expected.\n"), fname, 1);
        return 1;
    }

    // get first element as a string
    sciErr = getListItemAddress(pvApiCtx, piAddrTree, 1, &piAddrTreeStr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddrTreeStr, &strItem1))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: First element must be a string.\n"), fname, 1);
        return 1;
    }

    /*check tree structure */
    if (strcmp(strItem1, TREE_REF_NAME))
    {
        freeAllocatedSingleString(strItem1);
        Scierror(999, _("%s: Wrong type for input argument #%d: A Tree expected.\n"), fname, 1);
        return 1;
    }

    freeAllocatedSingleString(strItem1);

    // get the second element as a mlist
    sciErr = getListItemAddress(pvApiCtx, piAddrTree, 2, &piAddrTreeMList);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (isMListType(pvApiCtx, piAddrTreeMList) == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Second element must be a mlist.\n"), fname, 1);
        printError(&sciErr, 0);
        return 1;
    }

    //Add node level
    szCurLevel += "1";
    StructList.push_back(szCurLevel);

    //get label name
    sciErr = getListItemAddress(pvApiCtx, piAddrTreeMList, 3, &piAddrLabel);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddrLabel, &strLabel))
    {
        Scierror(999, _("%s: Wrong type for element %d of input argument #%d: A string expected.\n"), fname, 3, 1);
        return 1;
    }

    StructList.push_back(strLabel);
    freeAllocatedSingleString(strLabel);

    //get Icon name
    sciErr = getListItemAddress(pvApiCtx, piAddrTreeMList, 4, &piAddrIcon);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddrIcon, &strIcon))
    {
        Scierror(999, _("%s: Wrong type for element %d of input argument #%d: A string expected.\n"), fname, 4, 1);
        return 1;
    }

    StructList.push_back(strIcon);
    freeAllocatedSingleString(strIcon);

    //get callback name
    sciErr = getListItemAddress(pvApiCtx, piAddrTreeMList, 5, &piAddrCallback);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddrCallback, &strCallback))
    {
        Scierror(999, _("%s: Wrong type for element %d of input argument #%d: A string expected.\n"), fname, 5, 1);
        return 1;
    }

    StructList.push_back(strCallback);
    freeAllocatedSingleString(strCallback);

    if (bParseListItem(pvApiCtx, piAddrTree, iItemCount, &StructList, szCurLevel))
    {
        Scierror(999, _("%s: Error in the tree parsing.\n"), fname, 1);
        return 1;
    }
    // Conversion Vector<string> to char **
    char **tab = NULL;
    size_t i = 0;

    size_t struct_size = StructList.size();
    tab = new char *[struct_size];

    for (i = 0; i < struct_size; ++i)
    {
        tab[i] = strdup(StructList.at(i).c_str());
    }

    try
    {
        //Java
        org_scilab_modules_gui_tree::ScilabDisplayTree::scilabDisplayTree(getScilabJavaVM(), tab, (int)struct_size);
    }
    catch (const GiwsException::JniException & e)
    {
        Scierror(999, _("%s: A Java exception arisen:\n%s"), fname, e.whatStr().c_str());
        return FALSE;
    }

    //Free
    for (i = 0; i < struct_size; ++i)
    {
        FREE(tab[i]);
    }

    delete[]tab;
    tab = NULL;

    return 0;
}
