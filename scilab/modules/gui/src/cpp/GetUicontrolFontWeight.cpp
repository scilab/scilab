/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Get the font weight of an uicontrol 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUicontrolFontWeight.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolFontWeight(sciPointObj* sciObj)
{
  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      // Get the font weight from Scilab object
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
          Scierror(999, _("Wrong value for '%s' property: 'light', 'normal', 'demi' or 'bold' expected.\n"), "FontWeight");
          return FALSE;
        }
    }
  else
    {
      Scierror(999, _("No '%s' property for this object.\n"), "FontWeight");
      return FALSE;
    }
}
