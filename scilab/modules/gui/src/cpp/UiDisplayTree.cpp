/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Han Dong
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "UiDisplayTree.hxx"

using namespace org_scilab_modules_gui_bridge;

void createUiDisplayTree(sciPointObj* sciObj)
{
  pUICONTROL_FEATURE(sciObj)->hashMapIndex = CallScilabBridge::newUiDisplayTree(getScilabJavaVM());
}

int setCurentFigureAsUiDisplayTreeParent(sciPointObj* sciObj)
{
// ???
#if 0
  int parentFigureIndex = 0;

  // Scilab list of parent
  sciAddThisToItsParent(sciObj, sciGetCurrentFigure());

  // Java objects
  parentFigureIndex = sciGetNum(sciGetCurrentFigure());
  CallScilabBridge::setUiDisplayTreeParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);
#endif
  return SET_PROPERTY_SUCCEED;
}
