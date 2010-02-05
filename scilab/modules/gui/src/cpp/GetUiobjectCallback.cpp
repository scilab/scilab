/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Get the callback string of an uicontrol or uimenu
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUiobjectCallback.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUiobjectCallback(sciPointObj* sciObj)
{
  if (sciGetEntityType( sciObj ) == SCI_UIMENU)
    {
      /* Do not read from Java because can be diffucult to get back the callback */

      if (pUIMENU_FEATURE(sciObj)->callback == NULL) /* Callback not set */
        {
          return sciReturnString("");
        }
      else /* Callback has been set */
        {
          return sciReturnString(pUIMENU_FEATURE(sciObj)->callback);
        }
    }
  else if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      /* Do not read from Java because can be diffucult to get back the callback */

      if (pUICONTROL_FEATURE(sciObj)->callback == NULL) /* Callback not set */
        {
          return sciReturnString("");
        }
      else /* Callback has been set */
        {
          return sciReturnString(pUICONTROL_FEATURE(sciObj)->callback);
        }
    }
  else
    {
      Scierror(999, const_cast<char*>(_("No '%s' property for this object.\n")),
 "Callback");
      return FALSE;
    }
}

