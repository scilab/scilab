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

#ifndef __UIWIDGETTOOLS_HXX__
#define __UIWIDGETTOOLS_HXX__

#include <cstring>

namespace org_scilab_modules_gui_uiwidget
{

class UIWidgetTools
{

public:
    static bool isUIWidget(int * mlist, void * pvApiCtx);

    static int getUIWidgetId(int * mlist, void * pvApiCtx);

    static int createOnScilabStack(const int uid, const int pos, void * pvApiCtx);
};
}

#endif // __UIWIDGETTOOLS_HXX__
