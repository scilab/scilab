/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Sets the font size of an uicontrol object */

#include "SetUicontrolFontSize.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolFontSize(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  /* Font weight can be points, normalized, inches, centimeters or pixels */

  double fontSizeDouble = 0.0; 
  int fontSizeInt = 0.0; 

  if (valueType == sci_matrix)
    {
      if(nbCol != 1 || nbRow != 1)
        {
          /* Wrong value size */
          sciprint(_("%s property value must be single value.\n"), "FontSize");
          return SET_PROPERTY_ERROR;
        }
      
      fontSizeDouble = getDoubleFromStack(stackPointer);

      /* Compute the correct value according to FontUnits */
      switch(pUICONTROL_FEATURE(sciObj)->fontUnits)
        {
        case POINTS_UNITS:
          fontSizeInt = (int)fontSizeDouble;
          break;
        case NORMALIZED_UNITS:
          // TODO get correct value
          sciprint(_("Conversion of FontSize from Normalized units is not implemented.\n"));
          fontSizeInt = (int)fontSizeDouble;
          break;
        case INCHES_UNITS:
          fontSizeInt = (int)fontSizeDouble * 72;
          break;
        case CENTIMETERS_UNITS:
          fontSizeInt = (int)fontSizeDouble * 72 * 2.54;
          break;
        case PIXELS_UNITS:
          // TODO get correct value
          sciprint(_("Conversion of FontSize from Pixels units is not implemented.\n"));
          fontSizeInt = (int)fontSizeDouble;
          break;
        default:
          /* Wrong string format */
          sciprint(_("FontUnits property value must be a single string: points, normalized, inches, centimeters or pixels.\n"));
          return SET_PROPERTY_ERROR;
        }
      
      /* Send the value to java */
      switch(pUICONTROL_FEATURE(sciObj)->style)
        {
        case SCI_PUSHBUTTON:
          CallScilabBridge::setPushButtonFontSize(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex, fontSizeInt);
          return SET_PROPERTY_SUCCEED;
        default:
          /* Unimplmented uicontrol style */
          sciprint(_("No %s property for uicontrols of style: %s.\n"), "FontSize", UicontrolStyleToString(pUICONTROL_FEATURE(sciObj)->style));
          return SET_PROPERTY_ERROR;
        }
    }
  else
    {
      /* Wrong datatype */
      sciprint(_("%s property value must be single value.\n"), "FontSize");
      return SET_PROPERTY_ERROR;
    }

}

