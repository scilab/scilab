/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ContextMenu.hxx"

using namespace org_scilab_modules_gui_bridge;

// char* createContextMenu(char **menuLabels, int nbLabels)
// {
//   return CallScilabBridge::newContextMenu(getScilabJavaVM(), menuLabels, nbLabels);
// }

void createContextMenu(sciPointObj* sciObj)
{
  pUICONTEXTMENU_FEATURE(sciObj)->hashMapIndex = CallScilabBridge::newContextMenu(getScilabJavaVM());
}

char* uiWaitContextMenu(sciPointObj* sciObj)
{
  /* TO DO : create a copy or move sci_uiwait to cpp to do a delete */
  return CallScilabBridge::displayAndWaitContextMenu(getScilabJavaVM(), pUICONTEXTMENU_FEATURE(sciObj)->hashMapIndex);
}

