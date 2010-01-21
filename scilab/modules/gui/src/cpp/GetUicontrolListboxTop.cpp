/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Get the listbox top property of an uicontrol
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUicontrolListboxTop.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolListboxTop(sciPointObj* sciObj)
{
  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      if (pUICONTROL_FEATURE(sciObj)->listboxTop == NULL)
        {
          return sciReturnEmptyMatrix();
        }
      else
        {
          return sciReturnRowVectorFromInt(pUICONTROL_FEATURE(sciObj)->listboxTop, 1);
        }
    }
  else
    {
      Scierror(999, const_cast<char*>(_("No '%s' property for this object.\n")), "ListboxTop");
      return FALSE;
    }
}
