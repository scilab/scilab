/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Sets the vertical alignment of an uicontrol object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUicontrolVerticalAlignment.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolVerticalAlignment(sciPointObj* sciObj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
  /* VerticalAlignment can be top, middle or bottom */
  
  char * alignment = NULL;

  if (sciGetEntityType( sciObj ) != SCI_UICONTROL)
    {
      sciprint(_("No '%s' property for this object.\n"), "VerticalAlignment");
      return SET_PROPERTY_ERROR;
    }

  if (valueType == sci_strings)
    {
      if(nbCol != 1 || nbRow == 0)
        {
          /* Wrong string size */
          sciprint(_("Wrong size for '%s' property: '%s', '%s', or '%s' expected.\n"), "VerticalAlignment", "top", "middle", "bottom");
          return SET_PROPERTY_ERROR;
        }
      
      alignment = getStringFromStack(stackPointer);

      if (strcmp(alignment, "top") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->verticalAlignment = TOP_ALIGNMENT;
        }
      else if(strcmp(alignment, "middle") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->verticalAlignment = MIDDLE_ALIGNMENT;
        }
      else if(strcmp(alignment, "bottom") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->verticalAlignment = BOTTOM_ALIGNMENT;
        }
      else
        {
          /* Wrong string format */
          sciprint(_("Wrong value for '%s' property: '%s', '%s', or '%s' expected.\n"), "VerticalAlignment", "top", "middle", "bottom");
          return SET_PROPERTY_ERROR;
        }
      
      // Update Java object
      if (pUICONTROL_FEATURE(sciObj)->style != SCI_UIFRAME
          && pUICONTROL_FEATURE(sciObj)->style != SCI_EDIT
          && pUICONTROL_FEATURE(sciObj)->style != SCI_SLIDER
          && pUICONTROL_FEATURE(sciObj)->style != SCI_POPUPMENU
          && pUICONTROL_FEATURE(sciObj)->style != SCI_LISTBOX)
        {
          CallScilabBridge::setWidgetVerticalAlignment(getScilabJavaVM(), 
                                                       pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                       alignment);
        }
      return SET_PROPERTY_SUCCEED;
    }
  else
    {
      /* Wrong datatype */
      sciprint(_("Wrong type for '%s' property: '%s', '%s', or '%s' expected.\n"), "VerticalAlignment", "top", "middle", "bottom");
      return SET_PROPERTY_ERROR;
    }

}

