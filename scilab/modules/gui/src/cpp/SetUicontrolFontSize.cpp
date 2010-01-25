/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Sets the font size of an uicontrol object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUicontrolFontSize.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolFontSize(sciPointObj* sciObj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
  /* Font weight can be points, normalized, inches, centimeters or pixels */

  int fontSizeInt = 0; 

  if (sciGetEntityType( sciObj ) != SCI_UICONTROL)
    {
      Scierror(999, const_cast<char*>(_("No '%s' property for this object.\n")), "FontSize");
      return SET_PROPERTY_ERROR;
    }

  if (valueType == sci_matrix)
    {
      if(nbCol != 1 || nbRow != 1)
        {
          /* Wrong value size */
          Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A real expected.\n")), "FontSize");
          return SET_PROPERTY_ERROR;
        }

      fontSizeInt = ConvertToPoint((int) getDoubleFromStack(stackPointer), pUICONTROL_FEATURE(sciObj)->fontUnits, sciObj, FALSE);
      
      /* Send the value to java */
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrol */
        {
          CallScilabBridge::setFrameFontSize(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex, fontSizeInt);
        }
      else /* All other uicontrol styles */
        {
          CallScilabBridge::setWidgetFontSize(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex, fontSizeInt);
        }
      return SET_PROPERTY_SUCCEED;

    }
  else
    {
      /* Wrong datatype */
      Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: A real expected.\n")), "FontSize");
      return SET_PROPERTY_ERROR;
    }

}

