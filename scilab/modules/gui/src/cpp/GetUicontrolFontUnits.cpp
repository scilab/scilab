/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the font units of an uicontrol */

#include "GetUicontrolFontUnits.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolFontUnits(sciPointObj* sciObj)
{
  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      // Get the font weight from Scilab object
      switch(pUICONTROL_FEATURE(sciObj)->style)
        {
        case SCI_PUSHBUTTON:
        case SCI_EDIT:
          switch(pUICONTROL_FEATURE(sciObj)->fontUnits)
            {
            case POINTS_UNITS:
              return sciReturnString("points");
            case NORMALIZED_UNITS:
              return sciReturnString("normalized");
            case INCHES_UNITS:
              return sciReturnString("inches");
            case CENTIMETERS_UNITS:
              return sciReturnString("centimeters");
            case PIXELS_UNITS:
              return sciReturnString("pixels");
            default:
              sciprint(_("FontUnits property value must be a single string: points, normalized, inches, centimeters or pixels.\n"));
              sciReturnString(""); /* return an empty string */
              return FALSE;
            }
        default:
          sciprint(_("No %s property for uicontrols of style: %s.\n"), "FontUnits", UicontrolStyleToString(pUICONTROL_FEATURE(sciObj)->style));
          return FALSE;
        }
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "FontUnits");
      return FALSE;
    }
}
