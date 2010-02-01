/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Get the min property of an uicontrol 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUicontrolMin.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolMin(sciPointObj* sciObj)
{
  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      return sciReturnDouble(pUICONTROL_FEATURE(sciObj)->min);
    }
  else
    {
      Scierror(999, const_cast<char*>(_("No '%s' property for this object.\n")), "Min");
      return FALSE;
    }
}
