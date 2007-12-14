/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Set the label of an uicontrol or uimenu */

#include "SetLabelProperty.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetLabelProperty(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  // Label must be only one character string
  if (valueType != sci_strings) {
    // TODO display a message ?
    return SET_PROPERTY_ERROR;
  }
  if (nbCol != 1) {
    // TODO display a message ?
    return SET_PROPERTY_ERROR;
  }

  if (nbRow == 0) {
    // This case should never happen beacause if nbRow==0 then nbCol is also 0
    // TODO display a message ?
    return SET_PROPERTY_ERROR;
  }

  if (sciGetEntityType( sciObj ) == SCI_UIMENU)
    {
      // Send the label to Java
      CallScilabBridge::setMenuText(getScilabJavaVM(), pUIMENU_FEATURE(sciObj)->hashMapIndex, getStringFromStack(stackPointer));
    }
  else if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      if(strcmp(pUICONTROL_FEATURE(sciObj)->style, "pushbutton")==0)
        {
          // Send the label to Java
          CallScilabBridge::setPushButtonText(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex, getStringFromStack(stackPointer));
        }
      else
        {
          Scierror(999, _("Do not know how to set the label of an uicontrol of style: %s.\n"), pUICONTROL_FEATURE(sciObj)->style);
          return SET_PROPERTY_ERROR;
       }
    }
  return SET_PROPERTY_SUCCEED;
}

