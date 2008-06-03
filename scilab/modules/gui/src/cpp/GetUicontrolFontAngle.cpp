/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Get the font angle of an uicontrol
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUicontrolFontAngle.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolFontAngle(sciPointObj* sciObj)
{
  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      // Get the font angle from Scilab object
      switch(pUICONTROL_FEATURE(sciObj)->fontAngle)
        {
        case NORMAL_FONT:
          return sciReturnString("normal");
        case ITALIC_FONT:
          return sciReturnString("italic");
        case OBLIQUE_FONT:
          return sciReturnString("oblique");
        default:
          sciprint(_("Wrong value for '%s' property: '%s', '%s' or '%s' expected.\n"), "FontAngle", "normal", "italic", "oblique");
          return FALSE;
        }
    }
  else
    {
      sciprint(_("No '%s' property for this object.\n"), "FontAngle");
      return FALSE;
    }
}
