/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Sets the font angle of an uicontrol object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUicontrolFontAngle.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolFontAngle(sciPointObj* sciObj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
  /* Font angle can be normal, italic or oblique */

  char * fontAngle = NULL; 

  if (sciGetEntityType( sciObj ) != SCI_UICONTROL)
    {
      sciprint(_("No '%s' property for this object.\n"), "FontAngle");
      return SET_PROPERTY_ERROR;
    }

  if (valueType == sci_strings)
    {
      if(nbCol != 1 || nbRow == 0)
        {
          /* Wrong string size */
          sciprint(_("Wrong size for '%s' property: '%s', '%s' or '%s' expected.\n"), "FontAngle", "normal", "italic", "oblique");
          return SET_PROPERTY_ERROR;
        }
      
      fontAngle = getStringFromStack(stackPointer);

      if (strcmp(fontAngle, "normal") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->fontAngle = NORMAL_FONT;
        }
      else if(strcmp(fontAngle, "italic") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->fontAngle = ITALIC_FONT;
        }
      else if(strcmp(fontAngle, "oblique") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->fontAngle = OBLIQUE_FONT;
        }
      else
        {
          /* Wrong string format */
          sciprint(_("Wrong value for '%s' property: '%s', '%s' or '%s' expected.\n"), "FontAngle", "normal", "italic", "oblique");
          return SET_PROPERTY_ERROR;
        }
      
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrol */
        {
          CallScilabBridge::setFrameFontAngle(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex, fontAngle);     
        }
      else /* All other uicontrol styles */
        {
          CallScilabBridge::setWidgetFontAngle(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex, fontAngle);
        }
      return SET_PROPERTY_SUCCEED;
    }
  else
    {
      /* Wrong datatype */
      sciprint(_("Wrong type for '%s' property: '%s', '%s' or '%s' expected.\n"), "FontAngle", "normal", "italic", "oblique");
      return SET_PROPERTY_ERROR;
    }

}

