/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "UIWidgetTools.hxx"

extern "C"
{
#include "api_scilab.h"
}

#define __SCILAB_MLIST_UIWIDGET_TYPE__ "UIWidget"

namespace org_scilab_modules_uiwidget
{

bool UIWidgetTools::isUIWidget(int * mlist, void * pvApiCtx)
{
    char * mlist_type[2];
    int type;
    int rows, cols;
    int lengths[2];

    SciErr err = getVarType(pvApiCtx, mlist, &type);

    if (err.iErr || type != sci_mlist)
    {
        return false;
    }

    err = getMatrixOfStringInList(pvApiCtx, mlist, 1, &rows, &cols, 0, 0);
    if (err.iErr || rows * cols != 2)
    {
        return false;
    }

    err = getMatrixOfStringInList(pvApiCtx, mlist, 1, &rows, &cols, lengths, 0);
    if (err.iErr)
    {
        return false;
    }

    for (int i = 0; i < 2; i++)
    {
        mlist_type[i] = new char[lengths[i] + 1];
    }

    err = getMatrixOfStringInList(pvApiCtx, mlist, 1, &rows, &cols, lengths, mlist_type);
    if (err.iErr)
    {
        return false;
    }

    bool ret = !strcmp(mlist_type[0], __SCILAB_MLIST_UIWIDGET_TYPE__) && !strcmp(mlist_type[1], "_id");

    for (int i = 0; i < 2; i++)
    {
        delete[] mlist_type[i];
    }

    return ret;
}

int UIWidgetTools::getUIWidgetId(int * mlist, void * pvApiCtx)
{
    int * id = 0;
    int row, col;

    SciErr err = getMatrixOfInteger32InList(pvApiCtx, mlist, 2, &row, &col, &id);
    if (err.iErr)
    {
        return -1;
    }

    return *id;
}

int UIWidgetTools::createOnScilabStack(const int uid, const int pos, void * pvApiCtx)
{
    static const char * fields[] = {__SCILAB_MLIST_UIWIDGET_TYPE__, "_id"};
    int * mlistaddr = 0;
    SciErr err;

    err = createMList(pvApiCtx, pos, 2, &mlistaddr);
    if (err.iErr)
    {
        return 0;
    }

    err = createMatrixOfStringInList(pvApiCtx, pos, mlistaddr, 1, 1, 2, fields);
    if (err.iErr)
    {
        return 0;
    }

    err = createMatrixOfInteger32InList(pvApiCtx, pos, mlistaddr, 2, 1, 1, &uid);
    if (err.iErr)
    {
        return 0;
    }

    return 1;
}
}
