/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the font size of an uicontrol */

#include "GetUicontrolFontSize.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolFontSize(sciPointObj* sciObj)
{
  double fontSize = 0.0;

  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      // Get the font weight from Scilab object
      switch(pUICONTROL_FEATURE(sciObj)->style)
        {
        case SCI_PUSHBUTTON:
          fontSize = (double)CallScilabBridge::getPushButtonFontSize(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex);
          break;
        default:
          sciprint(_("No %s property for uicontrols of style: %s.\n"), "FontSize", UicontrolStyleToString(pUICONTROL_FEATURE(sciObj)->style));
          return FALSE;
        }

       /* Compute the correct value according to FontUnits */
      switch(pUICONTROL_FEATURE(sciObj)->fontUnits)
        {
        case POINTS_UNITS:
          fontSize = fontSize;
          break;
        case NORMALIZED_UNITS:
          // TODO get correct value
          sciprint(_("Conversion of FontSize to Normalized units is not implemented.\n"));
          fontSize = fontSize;
          break;
        case INCHES_UNITS:
          fontSize = fontSize / 72;
          break;
        case CENTIMETERS_UNITS:
          fontSize = fontSize / 72 / 2.54;
          break;
        case PIXELS_UNITS:
          // TODO get correct value
          sciprint(_("Conversion of FontSize to Pixels units is not implemented.\n"));
          fontSize = fontSize;
          break;
        default:
          /* Wrong string format */
          sciprint(_("FontUnits property value must be a single string: points, normalized, inches, centimeters or pixels.\n"));
          return FALSE;
        }
 
      return sciReturnDouble(fontSize);
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "FontSize");
      return FALSE;
    }
}
