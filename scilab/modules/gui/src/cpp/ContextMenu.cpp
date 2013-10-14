/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "ContextMenu.hxx"

using namespace org_scilab_modules_gui_bridge;

char *uiWaitContextMenu(int iObjUID)
{
    /* TO DO : create a copy or move sci_uiwait to cpp to do a delete */
    return CallScilabBridge::displayAndWaitContextMenu(getScilabJavaVM(), iObjUID);
}
