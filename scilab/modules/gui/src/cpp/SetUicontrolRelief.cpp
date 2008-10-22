/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Sets the relief of an uicontrol object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUicontrolRelief.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolRelief(sciPointObj* sciObj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
  /* Relief can be flat, groove, raised, ridge, solid or sunken */

  char * relief = NULL;
  
  if (sciGetEntityType( sciObj ) != SCI_UICONTROL)
    {
      sciprint(_("No '%s' property for this object.\n"), "Relief");
      return SET_PROPERTY_ERROR;
    }

  if (valueType == sci_strings)
    {
      if(nbCol != 1 || nbRow == 0)
        {
          /* Wrong string size */
          sciprint(_("Wrong size for '%s' property: '%s', '%s', '%s', '%s', '%s' or '%s' expected.\n"), "Relief", "flat", "groove", "raised", "ridge", "solid", "sunken");
          return SET_PROPERTY_ERROR;
        }
      
      relief = getStringFromStack(stackPointer);

      if (strcmp(relief, "flat") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->relief = FLAT_RELIEF;
        }
      else if(strcmp(relief, "groove") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->relief = GROOVE_RELIEF;
        }
      else if(strcmp(relief, "raised") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->relief = RAISED_RELIEF;
        }
      else if(strcmp(relief, "ridge") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->relief = RIDGE_RELIEF;
        }
      else if(strcmp(relief, "solid") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->relief = SOLID_RELIEF;
        }
      else if(strcmp(relief, "sunken") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->relief = SUNKEN_RELIEF;
        }
      else
        {
          /* Wrong string format */
          sciprint(_("Wrong value for '%s' property: '%s', '%s', '%s', '%s', '%s' or '%s' expected.\n"), "Relief", "flat", "groove", "raised", "ridge", "solid", "sunken");
          return SET_PROPERTY_ERROR;
        }
      
      switch(pUICONTROL_FEATURE(sciObj)->style)
        {
        case SCI_UIFRAME:
          // Change the relief of the Java object
          CallScilabBridge::setFrameRelief(getScilabJavaVM(), 
                                           pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                           relief);
          return SET_PROPERTY_SUCCEED;
        default: /* All Widgets */
          // Change the relief of the Java object
          CallScilabBridge::setWidgetRelief(getScilabJavaVM(), 
                                            pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                            relief);
          return SET_PROPERTY_SUCCEED;
        }
    }
  else
    {
      /* Wrong datatype */
      sciprint(_("Wrong type for '%s' property: '%s', '%s', '%s', '%s', '%s' or '%s' expected.\n"), "Relief", "flat", "groove", "raised", "ridge", "solid", "sunken");
      return SET_PROPERTY_ERROR;
    }

}

