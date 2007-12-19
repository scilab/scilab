/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Set the callback of an uicontrol or uimenu */

#include "SetUiobjectCallback.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUiobjectCallback(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  // Label must be only one character string
  if (valueType != sci_strings) {
    sciprint(_("%s property value must be a single string.\n"), "Callback");
    return SET_PROPERTY_ERROR;
  }
  if (nbCol != 1) {
    sciprint(_("%s property value must be a single string.\n"), "Callback");
    return SET_PROPERTY_ERROR;
  }

  if (nbRow == 0) {
    // This case should never happen because if nbRow==0 then nbCol is also 0
    sciprint(_("%s property value must be a single string.\n"), "Callback");
    return SET_PROPERTY_ERROR;
  }

  if (sciGetEntityType( sciObj ) == SCI_UIMENU)
    {
      // Send the label to Java
      CallScilabBridge::setMenuCallback(getScilabJavaVM(), pUIMENU_FEATURE(sciObj)->hashMapIndex, getStringFromStack(stackPointer));
    }
  else if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      if(strcmp(pUICONTROL_FEATURE(sciObj)->style, "pushbutton")==0)
        {
          // Send the label to Java
          CallScilabBridge::setPushButtonCallback(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex, getStringFromStack(stackPointer));
        }
      else
        {
          sciprint(_("No %s property for uicontrols of style: %s.\n"), "Callback", pUICONTROL_FEATURE(sciObj)->style);
          return SET_PROPERTY_ERROR;
       }
    }

  sciprint(_("No %s property for this object.\n"), "Callback");
  return SET_PROPERTY_SUCCEED;
}

