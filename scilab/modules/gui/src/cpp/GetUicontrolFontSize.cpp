/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Get the font size of an uicontrol
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUicontrolFontSize.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolFontSize(sciPointObj* sciObj)
{
  double fontSize = 0.0;

  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      // Get the font size from Scilab object
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrol */
        {
          fontSize = (double)CallScilabBridge::getFrameFontSize(getScilabJavaVM(), 
                                                                 pUICONTROL_FEATURE(sciObj)->hashMapIndex);
        }
      else /* All other uicontrol styles */
        {
          fontSize = (double)CallScilabBridge::getWidgetFontSize(getScilabJavaVM(), 
                                                                 pUICONTROL_FEATURE(sciObj)->hashMapIndex);
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
