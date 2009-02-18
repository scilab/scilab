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
  int fontSize = 0;

  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      // Get the font size from Scilab object
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrol */
        {
          fontSize = CallScilabBridge::getFrameFontSize(getScilabJavaVM(), 
                                                                 pUICONTROL_FEATURE(sciObj)->hashMapIndex);
        }
      else /* All other uicontrol styles */
        {
          fontSize = CallScilabBridge::getWidgetFontSize(getScilabJavaVM(), 
                                                                 pUICONTROL_FEATURE(sciObj)->hashMapIndex);
        }

      return sciReturnDouble(ConvertFromPoint(fontSize, pUICONTROL_FEATURE(sciObj)->fontUnits, sciObj, FALSE));
    }
  else
    {
      Scierror(999, _("No '%s' property for this object.\n"), "FontSize");
      return FALSE;
    }
}
