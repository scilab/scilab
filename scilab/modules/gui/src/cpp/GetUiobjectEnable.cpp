/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Get the callback string of an uicontrol or uimenu
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUiobjectEnable.hxx"

#ifdef _MSC_VER
#include "strdup_Windows.h"
#endif

using namespace org_scilab_modules_gui_bridge;

int GetUiobjectEnable(sciPointObj* sciObj)
{
  if (sciGetEntityType( sciObj ) == SCI_UIMENU)
    {
      if (CallScilabBridge::isWidgetEnable(getScilabJavaVM(), pUIMENU_FEATURE(sciObj)->hashMapIndex))
        {
          return sciReturnString(strdup("on"));
        }
      else
        {
          return sciReturnString(strdup("off"));
        }
    }
  else if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME)
        {
          if (CallScilabBridge::isFrameEnable(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex))
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
          if (CallScilabBridge::isWidgetEnable(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex))
            {
              return sciReturnString(strdup("on"));
            }
          else
            {
              return sciReturnString(strdup("off"));
            }
        }
    }
  else
    {
      Scierror(999, const_cast<char*>(_("No '%s' property for this object.\n")), "Enable");
      return FALSE;
    }
}

