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
		  int ret = 0;
		  char *text = CallScilabBridge::getFrameFontName(getScilabJavaVM(),
                                                                 pUICONTROL_FEATURE(sciObj)->hashMapIndex);

          ret = sciReturnString(text);
		  delete [] text;
		  return ret;
        }
      else/* All other uicontrol style */
        {
		  int ret = 0;
		  char *text = CallScilabBridge::getWidgetFontName(getScilabJavaVM(),pUICONTROL_FEATURE(sciObj)->hashMapIndex);
          ret = sciReturnString(text);
		  delete [] text;
		  return ret;
        }
     }
  else
    {
      sciprint(_("No '%s' property for this object.\n"), "FontName");
      return FALSE;
    }
}

