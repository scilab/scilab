/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the callback string of an uicontrol or uimenu */

#include "GetUiobjectCallback.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUiobjectCallback(sciPointObj* sciObj)
{
  if (sciGetEntityType( sciObj ) == SCI_UIMENU)
    {
      // Get the label from Java
      return sciReturnString(CallScilabBridge::getWidgetCallback(getScilabJavaVM(), pUIMENU_FEATURE(sciObj)->hashMapIndex));
    }
  else if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      // Get the label from Java
      switch(pUICONTROL_FEATURE(sciObj)->style)
        {
        case SCI_PUSHBUTTON:
        case SCI_EDIT:
          return sciReturnString(CallScilabBridge::getWidgetCallback(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex));
        default:
          sciprint(_("No %s property for uicontrols of style: %s.\n"), "Callback", UicontrolStyleToString(pUICONTROL_FEATURE(sciObj)->style));
          return FALSE;
        }
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "Callback");
      return FALSE;
    }
}

