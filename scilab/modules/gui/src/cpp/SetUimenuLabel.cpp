/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Set the label of anuimenu  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUimenuLabel.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUimenuLabel(sciPointObj* sciObj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
  // Label must be only one character string
  if (valueType != sci_strings) {
    Scierror(999, _("Wrong type for '%s' property: A string expected.\n"), "Label");
    return SET_PROPERTY_ERROR;
  }
  if (nbCol != 1) {
    Scierror(999, _("Wrong size for '%s' property: A string expected.\n"), "Label");
    return SET_PROPERTY_ERROR;
  }

  if (nbRow == 0) {
    // This case should never happen beacause if nbRow==0 then nbCol is also 0
    Scierror(999, _("Wrong size for '%s' property: A string expected.\n"), "Label");
    return SET_PROPERTY_ERROR;
  }

  if (sciGetEntityType( sciObj ) == SCI_UIMENU)
    {
      // Send the label to Java
      CallScilabBridge::setWidgetText(getScilabJavaVM(), pUIMENU_FEATURE(sciObj)->hashMapIndex, getStringFromStack(stackPointer));
      return SET_PROPERTY_SUCCEED;
    }
  else
    {
      Scierror(999, _("No '%s' property for this object.\n"), "Label");
      return SET_PROPERTY_ERROR;
    }
}

