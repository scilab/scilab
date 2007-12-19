/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the font weight of an uicontrol */

#include "GetUicontrolFontWeight.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolFontWeight(sciPointObj* sciObj)
{
  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      // Get the font weight from Scilab object
      switch(pUICONTROL_FEATURE(sciObj)->style)
        {
        case SCI_PUSHBUTTON:
          switch(pUICONTROL_FEATURE(sciObj)->fontWeight)
            {
            case LIGHT_FONT:
              return sciReturnString("light");
            case NORMAL_FONT:
              return sciReturnString("normal");
            case DEMI_FONT:
              return sciReturnString("demi");
            case BOLD_FONT:
              return sciReturnString("bold");
            default:
              sciprint(_("FontWeight property value must be a single string: light, normal, demi or bold.\n"));
              return FALSE;
            }
        default:
          sciprint(_("No %s property for uicontrols of style: %s.\n"), "FontWeight", UicontrolStyleToString(pUICONTROL_FEATURE(sciObj)->style));
          return FALSE;
        }
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "FontWeight");
      return FALSE;
    }
}
