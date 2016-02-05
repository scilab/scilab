/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Vincent COUVERt
* Copyright (C) 2011 - DIGITEO - Clément DAVID
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
/*--------------------------------------------------------------------------*/
#include "Xcos.hxx"
#include "xcosUtilities.hxx"
#include "loadStatus.hxx"

extern "C"
{
#include "gw_xcos.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "freeArrayOfString.h"
#include "getScilabJavaVM.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_xcos;
/*--------------------------------------------------------------------------*/
int sci_warnBlockByUID(char *fname, void* pvApiCtx)
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
    set_loaded_status(XCOS_CALLED);
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
