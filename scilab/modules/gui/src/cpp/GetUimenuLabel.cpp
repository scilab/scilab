/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the label of an uimenu */

#include "GetUimenuLabel.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUimenuLabel(sciPointObj* sciObj)
{
  if (sciGetEntityType( sciObj ) == SCI_UIMENU)
    {
      // Get the label from Java
      return sciReturnString(CallScilabBridge::getWidgetText(getScilabJavaVM(), pUIMENU_FEATURE(sciObj)->hashMapIndex));
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "Label");
      return FALSE;
    }
}

