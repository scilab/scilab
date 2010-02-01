/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Get the horizontal alignment property of an uicontrol
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUicontrolHorizontalAlignment.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolHorizontalAlignment(sciPointObj* sciObj)
{
  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      switch(pUICONTROL_FEATURE(sciObj)->horizontalAlignment)
        {
        case LEFT_ALIGNMENT:
          return sciReturnString("left");
        case CENTER_ALIGNMENT:
          return sciReturnString("center");
        case RIGHT_ALIGNMENT:
          return sciReturnString("right");
        default:
          Scierror(999, const_cast<char*>(_("Wrong value for '%s' property: '%s', '%s' or '%s' expected.\n")), "HorizontalAlignment", "left", "center", "right");
          return FALSE;
        }
    }
  else
    {
      Scierror(999, const_cast<char*>(_("No '%s' property for this object.\n")), "HorizontalAlignment");
      return FALSE;
    }
}
