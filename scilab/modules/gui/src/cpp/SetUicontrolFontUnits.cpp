/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Sets the font units of an uicontrol object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUicontrolFontUnits.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolFontUnits(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  /* Font weight can be points, normalized, inches, centimeters or pixels */

  char * fontUnits = NULL; 

  double userEntry = 0.0;

  int oldFontSize = 0, newFontSize = 0;

  if (valueType == sci_strings)
    {
      if(nbCol != 1 || nbRow == 0)
        {
          /* Wrong string size */
          sciprint(_("FontUnits property value must be a single string: points, normalized, inches, centimeters or pixels.\n"));
          return SET_PROPERTY_ERROR;
        }
      
      fontUnits = getStringFromStack(stackPointer);

      /* Get "TRUE" font size from Java */
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrols */
        {
          oldFontSize = CallScilabBridge::getFrameFontSize(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex);       
        }
      else /* All other uicontrol styles */
        {
          oldFontSize = CallScilabBridge::getWidgetFontSize(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex);       
        }

      /* Retrieve the user value */
      userEntry = ConvertFromPoint(oldFontSize, pUICONTROL_FEATURE(sciObj)->fontUnits, sciObj); 

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

      /* Compute new size */
      newFontSize = ConvertToPoint(userEntry, pUICONTROL_FEATURE(sciObj)->fontUnits, sciObj);

      /* Send the nex size to Java */
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrols */
        {
          CallScilabBridge::setFrameFontSize(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex, newFontSize);       
        }
      else /* All other uicontrol styles */
        {
          CallScilabBridge::setWidgetFontSize(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex, newFontSize);       
        }
      return SET_PROPERTY_SUCCEED;
    }
  else
    {
      /* Wrong datatype */
      sciprint(_("FontUnits property value must be a single string: points, normalized, inches, centimeters or pixels.\n"));
      return SET_PROPERTY_ERROR;
    }

}

