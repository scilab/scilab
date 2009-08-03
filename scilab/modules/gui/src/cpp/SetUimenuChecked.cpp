/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUimenuChecked.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUimenuChecked(sciPointObj* sciObj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
  char *status = NULL;

  // Label must be only one character string
  if (valueType == sci_strings)
    {
      if (nbCol != 1) {
        Scierror(999, _("Wrong value for '%s' property: '%s' or '%s' expected.\n"), "Checked", "on", "off");
        return SET_PROPERTY_ERROR;
      }

      status = getStringFromStack(stackPointer);
      
      if (stricmp(status, "on")!=0 && stricmp(status, "off")!=0)
        {
        Scierror(999, _("Wrong value for '%s' property: '%s' or '%s' expected.\n"), "Checked", "on", "off");
        return SET_PROPERTY_ERROR;

        }
    }
  else
    {
      Scierror(999, _("Wrong value for '%s' property: '%s' or '%s' expected.\n"), "Checked", "on", "off");
      return SET_PROPERTY_ERROR;
    }
  
  // Send the value to Java
  if (sciGetEntityType( sciObj ) == SCI_UIMENU)
    {
      CallScilabBridge::setMenuChecked(getScilabJavaVM(),
                                       pUIMENU_FEATURE(sciObj)->hashMapIndex,
                                       stricmp(status, "on")==0);
      return SET_PROPERTY_SUCCEED;
    }
  else
    {
      Scierror(999, _("No '%s' property for this object.\n"), "Checked");
      return SET_PROPERTY_ERROR;
    }
}

