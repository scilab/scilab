/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Vincent COUVERt
* Copyright (C) 2011 - DIGITEO - Clément DAVID
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "Xcos.hxx"
#include "xcosUtilities.hxx"
extern "C"
{
#include "gw_xcos.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "getScilabJavaVM.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_xcos;
/*--------------------------------------------------------------------------*/
int sci_warnBlockByUID(char *fname, unsigned long fname_len)
{
    int i;

    CheckRhs(2, 2);
    CheckLhs(0, 1);

    char** path = NULL;
    int pathLength = 0;

    char* msg;

    /** read UID **/
    if (readVectorString(pvApiCtx, 1, &path, &pathLength, fname))
    {
        return 0;
    }

    /* read msg */
    if (readSingleString(pvApiCtx, 2, &msg, fname))
    {
        for (i = 0; i < pathLength; i++)
        {
            FREE(path[i]);
        }
        FREE(path);
        return 0;
    }

    /* call the implementation */
    Xcos::warnCellByUID(getScilabJavaVM(), path, pathLength, msg);

    for (i = 0; i < pathLength; i++)
    {
        FREE(path[i]);
    }
    FREE(path);
    freeAllocatedSingleString(msg);

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
