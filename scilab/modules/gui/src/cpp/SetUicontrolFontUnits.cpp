/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Sets the font units of an uicontrol object */

#include "SetUicontrolFontUnits.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolFontUnits(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  /* Font weight can be points, normalized, inches, centimeters or pixels */

  char * fontUnits = NULL; 

  if (valueType == sci_strings)
    {
      if(nbCol != 1 || nbRow == 0)
        {
          /* Wrong string size */
          sciprint(_("FontUnits property value must be a single string: points, normalized, inches, centimeters or pixels.\n"));
          return SET_PROPERTY_ERROR;
        }
      
      fontUnits = getStringFromStack(stackPointer);

      if (strcmp(fontUnits, "points") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->fontUnits = POINTS_UNITS;
        }
      else if(strcmp(fontUnits, "normalized") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->fontUnits = NORMALIZED_UNITS;
        }
      else if(strcmp(fontUnits, "inches") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->fontUnits = INCHES_UNITS;
        }
      else if(strcmp(fontUnits, "centimeters") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->fontUnits = CENTIMETERS_UNITS;
        }
      else if(strcmp(fontUnits, "pixels") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->fontUnits = PIXELS_UNITS;
        }
      else
        {
          /* Wrong string format */
          sciprint(_("FontUnits property value must be a single string: points, normalized, inches, centimeters or pixels.\n"));
          return SET_PROPERTY_ERROR;
        }
      
      switch(pUICONTROL_FEATURE(sciObj)->style)
        {
        case SCI_PUSHBUTTON:
          // TODO Change de Scilab of the Java font
          // CallScilabBridge::setPushButtonFontUnits(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex, fontUnits);
          return SET_PROPERTY_SUCCEED;
        default:
          /* Unimplmented uicontrol style */
          sciprint(_("No %s property for uicontrols of style: %s.\n"), "FontUnits", UicontrolStyleToString(pUICONTROL_FEATURE(sciObj)->style));
          return SET_PROPERTY_ERROR;
        }
    }
  else
    {
      /* Wrong datatype */
      sciprint(_("FontUnits property value must be a single string: points, normalized, inches, centimeters or pixels.\n"));
      return SET_PROPERTY_ERROR;
    }

}

