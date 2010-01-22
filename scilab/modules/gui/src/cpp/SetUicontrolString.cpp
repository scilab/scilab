/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Set the string property of an uicontrol 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUicontrolString.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolString(sciPointObj* sciObj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
  // Label must be a character string
  if (valueType != sci_strings) {
    Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: A string expected.\n")), "String");
    return SET_PROPERTY_ERROR;
  }

  if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {

      switch(pUICONTROL_FEATURE(sciObj)->style)
        {
        case SCI_UIFRAME:
          // String must be only one character string
          if (nbCol * nbRow > 1) {
            Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A string expected.\n")), "String");
            return SET_PROPERTY_ERROR;
          }
          // Send the label to Java
          CallScilabBridge::setFrameText(getScilabJavaVM(), 
                                         pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                         getStringMatrixFromStack(stackPointer)[0]);
          return SET_PROPERTY_SUCCEED;
        case SCI_LISTBOX:
          // String can be single value or vector of character
          if (nbCol > 1 && nbRow > 1)
            {
              Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A string or a vector of strings expected.\n")), "String");
              return SET_PROPERTY_ERROR;
            }
          else
            {
              CallScilabBridge::setListBoxText(getScilabJavaVM(), 
                                               pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                               getStringMatrixFromStack(stackPointer),
                                               nbCol * nbRow );
              return SET_PROPERTY_SUCCEED;
            }
        case SCI_POPUPMENU:
          // String can be single value or vector of character
          if (nbCol > 1 && nbRow > 1)
            {
              Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A string or a vector of strings expected.\n")), "String");
              return SET_PROPERTY_ERROR;
            }
          else
            {
              CallScilabBridge::setPopupMenuText(getScilabJavaVM(), 
                                                 pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                 getStringMatrixFromStack(stackPointer),
                                                 nbCol * nbRow );
              return SET_PROPERTY_SUCCEED;
            }
        default:
          // String must be only one character string
          if (nbCol * nbRow > 1) {
            Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A string expected.\n")), "String");
            return SET_PROPERTY_ERROR;
          }
          // Send the label to Java
          CallScilabBridge::setWidgetText(getScilabJavaVM(), 
                                          pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                          getStringMatrixFromStack(stackPointer)[0]);
          return SET_PROPERTY_SUCCEED;
        }
    }
  else
    {
      Scierror(999, const_cast<char*>(_("No '%s' property for this object.\n")), "String");
      return SET_PROPERTY_ERROR;
    }
}

