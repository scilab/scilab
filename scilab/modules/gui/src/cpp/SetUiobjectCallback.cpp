/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Set the callback of an uicontrol or uimenu
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <cstring>
#include "SetUiobjectCallback.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUiobjectCallback(sciPointObj* sciObj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
  char * cbString = NULL;
  int cbType = 0;

  int strNbRow = 0, strNbCol = 0;
  int typeNbRow = 0, typeNbCol = 0;
  int typeStackPointer = 0, stringStackPointer = 0;

  // Label must be only one character string
  if (valueType == sci_strings)
    {
      if (nbCol != 1) {
        Scierror(999, _("Wrong size for '%s' property: A string expected.\n"), "Callback");
        return SET_PROPERTY_ERROR;
      }
      
      cbString = getStringFromStack(stackPointer);
    }
  else if (valueType == sci_list)
    {
      if (nbRow * nbCol != 2)
        {
          Scierror(999, _("Wrong size for '%s' property: a 2-item list expected.\n"), "Callback");
          return SET_PROPERTY_ERROR;
        }

      GetListRhsVar((int)stackPointer, 1, MATRIX_OF_DOUBLE_DATATYPE, &typeNbRow, &typeNbCol, &typeStackPointer);
      if (typeNbRow * typeNbCol !=1)
        {
          Scierror(999, _("Wrong size for '%s' property: A real expected.\n"), "CallbackType");
          return SET_PROPERTY_ERROR;
        }
      else
        {
          cbType = (int) (*stk(typeStackPointer));
        }

      GetListRhsVar((int)stackPointer, 2, STRING_DATATYPE, &strNbRow, &strNbCol, &stringStackPointer);
      if (strNbCol !=1)
        {
          Scierror(999, _("Wrong size for '%s' property: A string expected.\n"), "Callback");
          return SET_PROPERTY_ERROR;
        }
      else
        {
          cbString = cstk(stringStackPointer);
        }
    }

  if (sciGetEntityType( sciObj ) == SCI_UIMENU)
    {
      // Send the callback to Java
      CallScilabBridge::setWidgetCallback(getScilabJavaVM(),
                                          pUIMENU_FEATURE(sciObj)->hashMapIndex,
                                          cbString,
                                          cbType);

      // Store the value in Scilab
      // Clear previous callback
      if (pUIMENU_FEATURE(sciObj)->callback != NULL)
        {
          delete[] (pUIMENU_FEATURE(sciObj)->callback);
        }

      // Set the new callback
      pUIMENU_FEATURE(sciObj)->callback = new char[strlen(getStringFromStack(stackPointer)) + 1];
      strcpy(pUIMENU_FEATURE(sciObj)->callback, getStringFromStack(stackPointer));

      pUIMENU_FEATURE(sciObj)->callbackType = cbType;

      return SET_PROPERTY_SUCCEED;
    }
  else if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      // Send the callback to Java
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrols */
        {
          CallScilabBridge::setFrameCallback(getScilabJavaVM(), 
                                             pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                             cbString,
                                             cbType);
        }
      else /* All other uicontrol styles */
        {
          CallScilabBridge::setWidgetCallback(getScilabJavaVM(), 
                                              pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                              cbString,
                                              cbType);
        }

      // Store the value in Scilab
      // Clear previous callback
      if (pUICONTROL_FEATURE(sciObj)->callback != NULL)
        {
          delete[] (pUICONTROL_FEATURE(sciObj)->callback);
        }

      // Set the new callback
      pUICONTROL_FEATURE(sciObj)->callback = new char[strlen(cbString) + 1];
      strcpy(pUICONTROL_FEATURE(sciObj)->callback, cbString);

      if (strcmp(pUICONTROL_FEATURE(sciObj)->callback, "") == 0)
        {
          pUICONTROL_FEATURE(sciObj)->callbackType = -1; /* Disabled */
        }
      else
        {
          pUICONTROL_FEATURE(sciObj)->callbackType = cbType;
        }

      return SET_PROPERTY_SUCCEED;
    }
  else
    {
      Scierror(999, _("No '%s' property for this object.\n"), "Callback");
      return SET_PROPERTY_ERROR;
    }
}

