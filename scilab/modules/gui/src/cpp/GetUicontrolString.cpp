/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the string of an uicontrol */

#include "GetUicontrolString.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolString(sciPointObj* sciObj)
{  
  if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      // Get the string from Java
      return sciReturnString(CallScilabBridge::getWidgetText(getScilabJavaVM(),
                                                             pUICONTROL_FEATURE(sciObj)->hashMapIndex));
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "String");
      return FALSE;
    }
}
