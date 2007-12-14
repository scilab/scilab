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
}

int setCurentFigureAsPushButtonParent(sciPointObj* sciObj)
{
  int parentFigureIndex = 0;
  parentFigureIndex = sciGetNum(sciGetCurrentFigure());
  CallScilabBridge::setPushButtonParent(getScilabJavaVM(), parentFigureIndex, pUICONTROL_FEATURE(sciObj)->hashMapIndex);
  return SET_PROPERTY_SUCCEED;
}
