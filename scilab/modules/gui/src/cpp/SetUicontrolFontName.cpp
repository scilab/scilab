/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Set the font name of an uicontrol 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUicontrolFontName.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolFontName(sciPointObj* sciObj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
  // Font Name must be only one character string
  if (valueType != sci_strings) {
    Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: A string expected.\n")), "FontName");
    return SET_PROPERTY_ERROR;
  }
  if (nbCol != 1) {
    Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A string expected.\n")), "FontName");
    return SET_PROPERTY_ERROR;
  }

  if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {

      // The value is not tested because the visible Java Font is not changed if the name given is unknown
      
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrols */
        {
           CallScilabBridge::setFrameFontName(getScilabJavaVM(), 
                                                pUICONTROL_FEATURE(sciObj)->hashMapIndex, 
                                                getStringFromStack(stackPointer));
       }
      else /* All other uicontrol styles */
        {
          CallScilabBridge::setWidgetFontName(getScilabJavaVM(), 
                                                pUICONTROL_FEATURE(sciObj)->hashMapIndex, 
                                                getStringFromStack(stackPointer));
        }
      
      return SET_PROPERTY_SUCCEED;
    }
  else
    {
      Scierror(999, const_cast<char*>(_("No '%s' property for this object.\n")), "FontName");
      return SET_PROPERTY_ERROR;
    }
}

