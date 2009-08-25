/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUimenuChecked.hxx"

#ifdef _MSC_VER
#include "strdup_Windows.h"
#endif

using namespace org_scilab_modules_gui_bridge;

int GetUimenuChecked(sciPointObj* sciObj)
{
  if (sciGetEntityType( sciObj ) == SCI_UIMENU)
    {
      if (CallScilabBridge::isMenuChecked(getScilabJavaVM(), pUIMENU_FEATURE(sciObj)->hashMapIndex))
        {
          return sciReturnString(strdup("on"));
        }
      else
        {
          return sciReturnString(strdup("off"));
        }
    }
  else
    {
      Scierror(999, _("No '%s' property for this object.\n"), "Checked");
      return FALSE;
    }
}

