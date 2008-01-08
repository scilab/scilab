/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#include "EditBox.hxx"

using namespace org_scilab_modules_gui_bridge;

void createEditBox(sciPointObj* sciObj)
{
  pUICONTROL_FEATURE(sciObj)->hashMapIndex = CallScilabBridge::newEditBox(getScilabJavaVM());
}

int setCurentFigureAsEditBoxParent(sciPointObj* sciObj)
{
  int parentFigureIndex = 0;
  
  // Scilab list of parent
  sciAddThisToItsParent(sciObj, sciGetCurrentFigure());
  
  // Java objects
  parentFigureIndex = sciGetNum(sciGetCurrentFigure());
  CallScilabBridge::setEditBoxParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);
  
  // Scilab default values
  CallScilabBridge::setWidgetPosition(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex, 20, sciGetHeight(sciGetCurrentFigure()) - 80, 40, 20);

  return SET_PROPERTY_SUCCEED;
}
