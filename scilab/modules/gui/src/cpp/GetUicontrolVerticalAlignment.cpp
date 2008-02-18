/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Get the vertical alignment property of an uicontrol 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUicontrolVerticalAlignment.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolVerticalAlignment(sciPointObj* sciObj)
{
  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      switch(pUICONTROL_FEATURE(sciObj)->verticalAlignment)
        {
        case TOP_ALIGNMENT:
          return sciReturnString("top");
        case MIDDLE_ALIGNMENT:
          return sciReturnString("middle");
        case BOTTOM_ALIGNMENT:
          return sciReturnString("bottom");
        default:
          sciprint(_("VerticalAlignment property value must be a single string: top, middle, or bottom.\n"));
          return FALSE;
        }
    }
  else
    {
      sciprint(_("No %s property for this object.\n"), "VerticalAlignment");
      return FALSE;
    }
}
