/* Copyright INRIA 2008 */
/* Vincent COUVERT */

#include "ListBox.hxx"

using namespace org_scilab_modules_gui_bridge;

void createListBox(sciPointObj* sciObj)
{
  pUICONTROL_FEATURE(sciObj)->hashMapIndex = CallScilabBridge::newListBox(getScilabJavaVM());
}

int setCurentFigureAsListBoxParent(sciPointObj* sciObj)
{
  int parentFigureIndex = 0;
  
  // Scilab list of parent
  sciAddThisToItsParent(sciObj, sciGetCurrentFigure());
  
  // Java objects
  parentFigureIndex = sciGetNum(sciGetCurrentFigure());
  CallScilabBridge::setListBoxParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);
  
  // Scilab default values
  CallScilabBridge::setWidgetPosition(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex, 20, sciGetHeight(sciGetCurrentFigure()) - 80, 40, 20);

  return SET_PROPERTY_SUCCEED;
}
