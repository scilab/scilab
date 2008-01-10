/* Copyright INRIA 2008 */
/* Vincent COUVERT */

#include "PopupMenu.hxx"

using namespace org_scilab_modules_gui_bridge;

void createPopupMenu(sciPointObj* sciObj)
{
  pUICONTROL_FEATURE(sciObj)->hashMapIndex = CallScilabBridge::newPopupMenu(getScilabJavaVM());
}

int setCurentFigureAsPopupMenuParent(sciPointObj* sciObj)
{
  int parentFigureIndex = 0;
  
  // Scilab list of parent
  sciAddThisToItsParent(sciObj, sciGetCurrentFigure());
  
  // Java objects
  parentFigureIndex = sciGetNum(sciGetCurrentFigure());
  CallScilabBridge::setPopupMenuParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);
  
  // Scilab default values
  CallScilabBridge::setWidgetPosition(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex, 20, sciGetHeight(sciGetCurrentFigure()) - 80, 40, 20);

  return SET_PROPERTY_SUCCEED;
}
