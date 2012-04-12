/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "Toolbar.hxx"

using namespace org_scilab_modules_gui_bridge;

void setToolbarVisible(char *pParentUID, BOOL status)
{
    CallScilabBridge::setToolbarVisible(getScilabJavaVM(), pParentUID, BOOLtobool(status));
}

BOOL isToolbarVisible(char *pParentUID)
{
    return booltoBOOL(CallScilabBridge::isToolbarVisible(getScilabJavaVM(), pParentUID));
}
