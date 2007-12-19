/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the label of an uicontrol or uimenu */

#include "GetLabelProperty.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetLabelProperty(sciPointObj* sciObj)
{
  if (sciGetEntityType( sciObj ) == SCI_UIMENU)
    {
      // Get the label from Java
      return sciReturnString(CallScilabBridge::getMenuText(getScilabJavaVM(), pUIMENU_FEATURE(sciObj)->hashMapIndex));
    }
  else if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      // Get the label from Java
      if(strcmp(pUICONTROL_FEATURE(sciObj)->style, "pushbutton")==0)
        {
          return sciReturnString(CallScilabBridge::getPushButtonText(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex));
        }
      else
        {
          Scierror(999, _("No label for uicontrols of style: %s.\n"), pUICONTROL_FEATURE(sciObj)->style);
          return FALSE;
        }
    }
  else
    {
      Scierror(999, _("No style for this kind of object.\n"));
      return FALSE;
    }
}

