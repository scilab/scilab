/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Sets the font weight of an uicontrol object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUicontrolFontWeight.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolFontWeight(sciPointObj* sciObj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
  /* Font weight can be light, normal, demi or bold */

  char * fontWeight = NULL; 

  if (sciGetEntityType( sciObj ) != SCI_UICONTROL)
    {
		Scierror(999, const_cast<char*>(_("No '%s' property for this object.\n")), "FontWeight");
      return SET_PROPERTY_ERROR;
    }

  if (valueType == sci_strings)
    {
      if(nbCol != 1)
        {
          /* Wrong string size */
          Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: '%s', '%s', '%s' or '%s' expected.\n")), "FontWeight", "light", "normal", "demi", "bold");
          return SET_PROPERTY_ERROR;
        }
      
      fontWeight = getStringFromStack(stackPointer);

      if (strcmp(fontWeight, "light") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->fontWeight = LIGHT_FONT;
        }
      else if(strcmp(fontWeight, "normal") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->fontWeight = NORMAL_FONT;
        }
      else if(strcmp(fontWeight, "demi") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->fontWeight = DEMI_FONT;
        }
      else if(strcmp(fontWeight, "bold") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->fontWeight = BOLD_FONT;
        }
      else
        {
          /* Wrong string format */
          Scierror(999, const_cast<char*>(_("Wrong value for '%s' property: '%s', '%s', '%s' or '%s' expected.\n")), "FontWeight", "light", "normal", "demi", "bold");
          return SET_PROPERTY_ERROR;
        }
      
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrols */
        {
           CallScilabBridge::setFrameFontWeight(getScilabJavaVM(), 
                                                pUICONTROL_FEATURE(sciObj)->hashMapIndex, 
                                                fontWeight);
       }
      else /* All other uicontrol styles */
        {
          CallScilabBridge::setWidgetFontWeight(getScilabJavaVM(), 
                                                pUICONTROL_FEATURE(sciObj)->hashMapIndex, 
                                                fontWeight);
        }
      return SET_PROPERTY_SUCCEED;
    }
  else
    {
      /* Wrong datatype */
      Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: '%s', '%s', '%s' or '%s' expected.\n")), "FontWeight", "light", "normal", "demi", "bold");
      return SET_PROPERTY_ERROR;
    }

}

