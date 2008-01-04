/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Set the string property of an uicontrol */

#include "SetUicontrolString.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolString(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  // Label must be only one character string
  if (valueType != sci_strings) {
    sciprint(_("%s property value must be a single string.\n"), "String");
    return SET_PROPERTY_ERROR;
  }
  if (nbCol != 1) {
    sciprint(_("%s property value must be a single string.\n"), "String");
    return SET_PROPERTY_ERROR;
  }

  if (nbRow == 0) {
    // This case should never happen beacause if nbRow==0 then nbCol is also 0
    sciprint(_("%s property value must be a single string.\n"), "String");
    return SET_PROPERTY_ERROR;
  }

  if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      switch(pUICONTROL_FEATURE(sciObj)->style)
        {
          // Send the label to Java
        case SCI_PUSHBUTTON:
          CallScilabBridge::setWidgetText(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex, getStringFromStack(stackPointer));
          return SET_PROPERTY_SUCCEED;
        default:
          sciprint(_("No %s property for uicontrols of style: %s.\n"), "String", UicontrolStyleToString(pUICONTROL_FEATURE(sciObj)->style));
          return SET_PROPERTY_ERROR;
       }
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "String");
      return SET_PROPERTY_ERROR;
    }
}

