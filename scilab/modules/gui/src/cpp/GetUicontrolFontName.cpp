/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Get the font name of an uicontrol 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUicontrolFontName.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolFontName(sciPointObj* sciObj)
{
  if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      // Get the font name from Java
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrol */
        {
          return sciReturnString(CallScilabBridge::getFrameFontName(getScilabJavaVM(),
                                                                 pUICONTROL_FEATURE(sciObj)->hashMapIndex));
        }
      else/* All other uicontrol style */
        {
          return sciReturnString(CallScilabBridge::getWidgetFontName(getScilabJavaVM(),
                                                                 pUICONTROL_FEATURE(sciObj)->hashMapIndex));
        }
     }
  else
    {
      sciprint(_("No %s property for this object.\n"), "FontName");
      return FALSE;
    }
}

