/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Set the string property of an uicontrol */

#include "SetUicontrolString.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolString(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  // Label must be a character string
  if (valueType != sci_strings) {
    sciprint(_("%s property value must be a single string.\n"), "String");
    return SET_PROPERTY_ERROR;
  }

  if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {

      switch(pUICONTROL_FEATURE(sciObj)->style)
        {
        case SCI_UIFRAME:
          // String must be only one character string
          if (nbCol * nbRow > 1) {
            sciprint(_("%s property value must be a single string.\n"), "String");
            return SET_PROPERTY_ERROR;
          }
          // Send the label to Java
          CallScilabBridge::setFrameText(getScilabJavaVM(), 
                                         pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                         getStringMatrixFromStack(stackPointer)[0]);
          return SET_PROPERTY_SUCCEED;
        case SCI_LISTBOX:
          // String can be single value or vector of character
          if (nbCol > 1 && nbRow > 1)
            {
              sciprint(_("%s property value must be a single string or a vector of strings.\n"), "String");
              return SET_PROPERTY_ERROR;
            }
          else
            {
              CallScilabBridge::setListBoxText(getScilabJavaVM(), 
                                               pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                               getStringMatrixFromStack(stackPointer),
                                               nbCol * nbRow );
              return SET_PROPERTY_SUCCEED;
            }
        case SCI_POPUPMENU:
          // String can be single value or vector of character
          if (nbCol > 1 && nbRow > 1)
            {
              sciprint(_("%s property value must be a single string or a vector of strings.\n"), "String");
              return SET_PROPERTY_ERROR;
            }
          else
            {
              CallScilabBridge::setPopupMenuText(getScilabJavaVM(), 
                                                 pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                 getStringMatrixFromStack(stackPointer),
                                                 nbCol * nbRow );
              return SET_PROPERTY_SUCCEED;
            }
        default:
          // String must be only one character string
          if (nbCol * nbRow > 1) {
            sciprint(_("%s property value must be a single string.\n"), "String");
            return SET_PROPERTY_ERROR;
          }
          // Send the label to Java
          CallScilabBridge::setWidgetText(getScilabJavaVM(), 
                                          pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                          getStringMatrixFromStack(stackPointer)[0]);
          return SET_PROPERTY_SUCCEED;
        }
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "String");
      return SET_PROPERTY_ERROR;
    }
}

