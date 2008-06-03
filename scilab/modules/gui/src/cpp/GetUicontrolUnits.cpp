/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Get the units of an uicontrol 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUicontrolUnits.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolUnits(sciPointObj* sciObj)
{
  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      // Get the font units from Scilab object
      switch(pUICONTROL_FEATURE(sciObj)->units)
        {
        case POINTS_UNITS:
          return sciReturnString("points");
        case NORMALIZED_UNITS:
          return sciReturnString("normalized");
        case INCHES_UNITS:
          return sciReturnString("inches");
        case CENTIMETERS_UNITS:
          return sciReturnString("centimeters");
        case PIXELS_UNITS:
          return sciReturnString("pixels");
        default:
          sciprint(_("Wrong value for '%s' property: '%s', '%s', '%s', '%s' or '%s' expected.\n"), "Units", "points", "normalized", "inches", "centimeters", "pixels");
          return FALSE;
        }
    }
  else
    {
      sciprint(_("No '%s' property for this object.\n"), "Units");
      return FALSE;
    }
}
