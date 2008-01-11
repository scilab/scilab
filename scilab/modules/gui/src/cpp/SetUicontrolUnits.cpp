/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Sets the units of an uicontrol object */

#include "SetUicontrolUnits.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolUnits(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  /* Units can be points, normalized, inches, centimeters or pixels */

  char * units = NULL; 

  if (valueType == sci_strings)
    {
      if(nbCol != 1 || nbRow == 0)
        {
          /* Wrong string size */
          sciprint(_("Units property value must be a single string: points, normalized, inches, centimeters or pixels.\n"));
          return SET_PROPERTY_ERROR;
        }
      
      units = getStringFromStack(stackPointer);

      if (strcmp(units, "points") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->units = POINTS_UNITS;
        }
      else if(strcmp(units, "normalized") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->units = NORMALIZED_UNITS;
        }
      else if(strcmp(units, "inches") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->units = INCHES_UNITS;
        }
      else if(strcmp(units, "centimeters") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->units = CENTIMETERS_UNITS;
        }
      else if(strcmp(units, "pixels") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->units = PIXELS_UNITS;
        }
      else
        {
          /* Wrong string format */
          sciprint(_("Units property value must be a single string: points, normalized, inches, centimeters or pixels.\n"));
          return SET_PROPERTY_ERROR;
        }
      
      // Update Java objects 
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrol */
        {
          // TODO Change size and dimensions of the frame
        }
      else /* All other uicontrol styles */
        {
          // TODO Change size and dimensions of the widget
        }
      return SET_PROPERTY_SUCCEED;
    }
  else
    {
      /* Wrong datatype */
      sciprint(_("Units property value must be a single string: points, normalized, inches, centimeters or pixels.\n"));
      return SET_PROPERTY_ERROR;
    }

}

