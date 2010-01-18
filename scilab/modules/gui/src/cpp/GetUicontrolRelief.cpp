/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Get the relief property of an uicontrol
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUicontrolRelief.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolRelief(sciPointObj* sciObj)
{
  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      switch(pUICONTROL_FEATURE(sciObj)->relief)
        {
        case FLAT_RELIEF:
          return sciReturnString("flat");
        case GROOVE_RELIEF:
          return sciReturnString("groove");
        case RAISED_RELIEF:
          return sciReturnString("raised");
        case RIDGE_RELIEF:
          return sciReturnString("ridge");
        case SOLID_RELIEF:
          return sciReturnString("solid");
        case SUNKEN_RELIEF:
          return sciReturnString("sunken");
        default:
          Scierror(999, const_cast<char*>(_("Wrong value for '%s' property: '%s', '%s', '%s', '%s', '%s' or '%s' expected.\n")), "Relief", "flat", "groove", "raised", "ridge", "solid", "sunken");
          return FALSE;
        }
    }
  else
    {
      Scierror(999, const_cast<char*>(_("No '%s' property for this object.\n")), "Relief");
      return FALSE;
    }
}
