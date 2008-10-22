/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Set the callback of an uicontrol or uimenu
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUiobjectVisible.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUiobjectVisible(sciPointObj* sciObj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
  char *status = NULL;

  // Label must be only one character string
  if (valueType == sci_strings)
    {
      if (nbCol != 1) {
        sciprint(_("Wrong value for '%s' property: '%s' or '%s' expected.\n"), "Visible", "on", "off");
        return SET_PROPERTY_ERROR;
      }

      status = getStringFromStack(stackPointer);
      
      if (stricmp(status, "on")!=0 && stricmp(status, "off")!=0)
        {
        sciprint(_("Wrong value for '%s' property: '%s' or '%s' expected.\n"), "Visible", "on", "off");
        return SET_PROPERTY_ERROR;

        }
    }
  else
    {
      sciprint(_("Wrong value for '%s' property: '%s' or '%s' expected.\n"), "Visible", "on", "off");
      return SET_PROPERTY_ERROR;
    }
  
  // Send the value to Java
  if (sciGetEntityType( sciObj ) == SCI_UIMENU)
    {
      CallScilabBridge::setWidgetVisible(getScilabJavaVM(),
                                          pUIMENU_FEATURE(sciObj)->hashMapIndex,
                                          stricmp(status, "on")==0);
      return SET_PROPERTY_SUCCEED;
    }
  else if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      // Send the callback to Java
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrols */
        {
          CallScilabBridge::setFrameVisible(getScilabJavaVM(),
                                            pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                            stricmp(status, "on")==0);
        }
      else /* All other uicontrol styles */
        {
          CallScilabBridge::setWidgetVisible(getScilabJavaVM(),
                                            pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                            stricmp(status, "on")==0);
        }
      return SET_PROPERTY_SUCCEED;
    }
  else
    {
      sciprint(_("No '%s' property for this object.\n"), "Visible");
      return SET_PROPERTY_ERROR;
    }
}

