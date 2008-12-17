/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Get the tag of an uicontrol or uimenu 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUiobjectTag.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUiobjectTag(sciPointObj* sciObj)
{
  if (sciGetEntityType( sciObj ) == SCI_UIMENU)
    {
      // Get the tag
      if (pUIMENU_FEATURE(sciObj)->tag == NULL)
        {
          return sciReturnString("");
        }
      else
        {
          return sciReturnString(pUIMENU_FEATURE(sciObj)->tag);
        }
    }
  else if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      // Get the tag from Scilab
      if (pUICONTROL_FEATURE(sciObj)->tag == NULL)
        {
          return sciReturnString("");
        }
      else
        {
          return sciReturnString(pUICONTROL_FEATURE(sciObj)->tag);
        }
    }
  else if (sciGetEntityType( sciObj ) == SCI_FIGURE)
    {
      // Get the tag from Scilab
      if (pFIGURE_FEATURE(sciObj)->tag == NULL)
        {
          return sciReturnString("");
        }
      else
        {
          return sciReturnString(pFIGURE_FEATURE(sciObj)->tag);
        }
    }
  else
    {
      Scierror(999, _("No '%s' property for this object.\n"), "Tag");
      return FALSE;
    }
}

