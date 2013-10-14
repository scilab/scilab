/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "UIWidget.hxx"
#include "UIWidgetTools.hxx"

extern "C"
{
#include "api_scilab.h"
#include "UIWidget.h"
#include "getScilabJavaVM.h"
}

namespace org_scilab_modules_gui_uiwidget
{

bool UIWidgetTools::isUIWidget(int * addr, void * pvApiCtx)
{
    if (isHandleType(pvApiCtx, addr))
    {
        long long hdl = 0;
        if (!getScalarHandle(pvApiCtx, addr, &hdl))
        {
            if (hdl < 0)
            {
                return true;
            }
        }
    }

    return false;
}

int UIWidgetTools::getUIWidgetId(int * addr, void * pvApiCtx)
{
    long long hdl = 0;
    if (!getScalarHandle(pvApiCtx, addr, &hdl))
    {
        return (int)(-hdl - 1);
    }

    return -1;
}

int UIWidgetTools::createOnScilabStack(const int uid, const int pos, void * pvApiCtx)
{
    return createScalarHandle(pvApiCtx, pos, (long long)(-uid - 1));
}
}

void deleteUIWidget(const long long hdl)
{
    org_scilab_modules_gui_uiwidget::UIWidget::uidelete(getScilabJavaVM(), (int)(-hdl - 1));
}

void deleteAllUIWidget()
{
    org_scilab_modules_gui_uiwidget::UIWidget::uideleteAll(getScilabJavaVM());
}

int isValidUIWidget(const long long hdl)
{
    return org_scilab_modules_gui_uiwidget::UIWidget::uiisValid(getScilabJavaVM(), (int)(-hdl - 1)) ? 1 : 0;
}

void showWindowUIWidget(const long long hdl)
{
    org_scilab_modules_gui_uiwidget::UIWidget::uishowWindow(getScilabJavaVM(), (int)(-hdl - 1));
}
