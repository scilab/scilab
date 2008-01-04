/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Set the font name of an uicontrol */

#include "SetUicontrolFontName.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolFontName(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  // Label must be only one character string
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
      // No test needed about the style because not implemented in Java code

      // Free old tag is exists
      if(pUICONTROL_FEATURE(sciObj)->fontName != NULL)
        {
          delete (pUICONTROL_FEATURE(sciObj)->fontName);
        }
      // Set the new tag
      pUICONTROL_FEATURE(sciObj)->fontName = new char[strlen(getStringFromStack(stackPointer)) + 1];
      strcpy(pUICONTROL_FEATURE(sciObj)->fontName, getStringFromStack(stackPointer));
      
      return SET_PROPERTY_SUCCEED;
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "FontName");
      return SET_PROPERTY_ERROR;
    }
}

