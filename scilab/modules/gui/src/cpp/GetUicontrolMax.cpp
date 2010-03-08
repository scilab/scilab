/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Get the max property of an uicontrol
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUicontrolMax.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolMax(sciPointObj* sciObj)
{
  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      return sciReturnDouble(pUICONTROL_FEATURE(sciObj)->max);
    }
  else
    {
      Scierror(999, const_cast<char*>(_("No '%s' property for this object.\n")), "Max");
      return FALSE;
    }
}
