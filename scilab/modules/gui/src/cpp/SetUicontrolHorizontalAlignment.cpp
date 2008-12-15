/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Sets the horizontal alignment of an uicontrol object  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUicontrolHorizontalAlignment.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolHorizontalAlignment(sciPointObj* sciObj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
  /* HorizontalAlignment can be left, center or right */
  
  char * alignment = NULL;

  if (sciGetEntityType( sciObj ) != SCI_UICONTROL)
    {
      Scierror(999, _("No '%s' property for this object.\n"), "HorizontalAlignment");
      return SET_PROPERTY_ERROR;
    }

  if (valueType == sci_strings)
    {
      if(nbCol != 1 || nbRow == 0)
        {
          /* Wrong string size */
          Scierror(999, _("Wrong size for '%s' property: '%s', '%s' or '%s' expected.\n"), "HorizontalAlignment", "left", "center", "right");
          return SET_PROPERTY_ERROR;
        }
      
      alignment = getStringFromStack(stackPointer);

      if (strcmp(alignment, "left") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->horizontalAlignment = LEFT_ALIGNMENT;
        }
      else if(strcmp(alignment, "center") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->horizontalAlignment = CENTER_ALIGNMENT;
        }
      else if(strcmp(alignment, "right") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->horizontalAlignment = RIGHT_ALIGNMENT;
        }
      else
        {
          /* Wrong string format */
          Scierror(999, _("Wrong value for '%s' property: '%s', '%s' or '%s' expected.\n"), "HorizontalAlignment", "left", "center", "right");
          return SET_PROPERTY_ERROR;
        }
      
      // Update Java object
      if (pUICONTROL_FEATURE(sciObj)->style != SCI_UIFRAME
          && pUICONTROL_FEATURE(sciObj)->style != SCI_SLIDER
          && pUICONTROL_FEATURE(sciObj)->style != SCI_POPUPMENU
          && pUICONTROL_FEATURE(sciObj)->style != SCI_LISTBOX)
        {
          CallScilabBridge::setWidgetHorizontalAlignment(getScilabJavaVM(), 
                                                       pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                         alignment);
        }
      return SET_PROPERTY_SUCCEED;
    }
  else
    {
      /* Wrong datatype */
      Scierror(999, _("Wrong type for '%s' property: '%s', '%s' or '%s' expected.\n"), "HorizontalAlignment", "left", "center", "right");
      return SET_PROPERTY_ERROR;
    }

}

