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


#include "ImageRenderer.hxx"

using namespace org_scilab_modules_gui_bridge;

void createImageRenderer(sciPointObj* sciObj)
{
  pUICONTROL_FEATURE(sciObj)->hashMapIndex = CallScilabBridge::newImageRenderer(getScilabJavaVM());
}

int setCurentFigureAsImageRendererParent(sciPointObj* sciObj)
{
  int parentFigureIndex = 0;
  
  // Scilab list of parent
  sciAddThisToItsParent(sciObj, sciGetCurrentFigure());
  
  // Java objects
  parentFigureIndex = sciGetNum(sciGetCurrentFigure());
  CallScilabBridge::setImageRendererParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);

  return SET_PROPERTY_SUCCEED;
}
