/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the callback string of an uicontrol or uimenu */

#include "GetUiobjectCallback.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUiobjectCallback(sciPointObj* sciObj)
{
  if (sciGetEntityType( sciObj ) == SCI_UIMENU)
    {
      // Get the callback from Java
      return sciReturnString(CallScilabBridge::getWidgetCallback(getScilabJavaVM(), pUIMENU_FEATURE(sciObj)->hashMapIndex));
    }
  else if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      // Get the callback from Java
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrol */
        {
          return sciReturnString(CallScilabBridge::getFrameCallback(getScilabJavaVM(),
                                                                     pUICONTROL_FEATURE(sciObj)->hashMapIndex));
        }
      else /* All other uicontrol styles */
        {
          return sciReturnString(CallScilabBridge::getWidgetCallback(getScilabJavaVM(),
                                                                     pUICONTROL_FEATURE(sciObj)->hashMapIndex));
        }
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "Callback");
      return FALSE;
    }
}

