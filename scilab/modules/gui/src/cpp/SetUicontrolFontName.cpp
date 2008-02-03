/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Set the font name of an uicontrol */

#include "SetUicontrolFontName.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolFontName(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  // Font Name must be only one character string
  if (valueType != sci_strings) {
    sciprint(_("%s property value must be a single string.\n"), "FontName");
    return SET_PROPERTY_ERROR;
  }
  if (nbCol != 1) {
    sciprint(_("%s col property value must be a single string.\n"), "FontName");
    return SET_PROPERTY_ERROR;
  }

  if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {

      // The value is not tested because the visible Java Font is not changed if the name given is unknown
      
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrols */
        {
           CallScilabBridge::setFrameFontName(getScilabJavaVM(), 
                                                pUICONTROL_FEATURE(sciObj)->hashMapIndex, 
                                                getStringFromStack(stackPointer));
       }
      else /* All other uicontrol styles */
        {
          CallScilabBridge::setWidgetFontName(getScilabJavaVM(), 
                                                pUICONTROL_FEATURE(sciObj)->hashMapIndex, 
                                                getStringFromStack(stackPointer));
        }
      
      return SET_PROPERTY_SUCCEED;
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "FontName");
      return SET_PROPERTY_ERROR;
    }
}

