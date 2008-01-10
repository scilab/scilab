/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the font angle of an uicontrol */

#include "GetUicontrolFontAngle.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolFontAngle(sciPointObj* sciObj)
{
  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      // Get the font weight from Scilab object
      switch(pUICONTROL_FEATURE(sciObj)->fontAngle)
        {
        case NORMAL_FONT:
          return sciReturnString("normal");
        case ITALIC_FONT:
          return sciReturnString("italic");
        case OBLIQUE_FONT:
          return sciReturnString("oblique");
        default:
          sciprint(_("FontAngle property value must be a single string: normal, italic or oblique.\n"));
          return FALSE;
        }
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "FontAngle");
      return FALSE;
    }
}
