/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the font name of an uicontrol */

#include "GetUicontrolFontName.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolFontName(sciPointObj* sciObj)
{
  if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      // Get the fontName from Scilab
      return sciReturnString(pUICONTROL_FEATURE(sciObj)->fontName);
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "FontName");
      return FALSE;
    }
}

