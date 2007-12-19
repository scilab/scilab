/* Copyright INRIA 2007 */
/* Vincent COUVERT */

#include "PushButton.hxx"
#include "getHandleDrawer.h"
#include "CallScilabBridge.hxx"
extern "C"{
#include "BOOL.h"
#include "getScilabJavaVM.h"
#include "CurrentObjectsManagement.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
}

using namespace org_scilab_modules_gui_bridge;

void createPushButton(sciPointObj* sciObj)
{
  pUICONTROL_FEATURE(sciObj)->hashMapIndex = CallScilabBridge::newPushButton(getScilabJavaVM());
  // Scilab default values
  pUICONTROL_FEATURE(sciObj)->fontWeight = NORMAL_FONT;
}

int setCurentFigureAsPushButtonParent(sciPointObj* sciObj)
{
  int parentFigureIndex = 0;
  
  // Scilab list of parent
  sciAddThisToItsParent(sciObj, sciGetCurrentFigure());
  
  // Java objects
  parentFigureIndex = sciGetNum(sciGetCurrentFigure());
  CallScilabBridge::setPushButtonParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);
  
  // Scilab default values
  CallScilabBridge::setPushButtonPosition(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex, 20, sciGetHeight(sciGetCurrentFigure()) - 80, 40, 20);

  return SET_PROPERTY_SUCCEED;
}
