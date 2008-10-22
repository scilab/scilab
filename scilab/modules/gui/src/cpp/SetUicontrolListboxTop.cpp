/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Sets the listbox top property of an uicontrol object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUicontrolListboxTop.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolListboxTop(sciPointObj* sciObj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
  int value = 0, nbValue = 0;

  if (valueType == sci_matrix)
    {
      if(nbCol > 1 || nbRow > 1)
        {
          /* Wrong value size */
          sciprint(_("Wrong size for '%s' property: A real expected.\n"), "ListboxTop");
          return SET_PROPERTY_ERROR;
        }

      value = (int) getDoubleFromStack(stackPointer);
    }
  else if (valueType == sci_strings) // Ascendant compatibility
    {
      if(nbCol > 1 || nbRow > 1)
        {
          /* Wrong value size */
          sciprint(_("Wrong size for '%s' property: A real expected.\n"), "ListboxTop");
          return SET_PROPERTY_ERROR;
        }

      nbValue = sscanf(getStringFromStack(stackPointer), "%d", &value);

      if(nbValue != 1)
        {
          /* Wrong value size */
          sciprint(_("Wrong value for '%s' property: A real expected.\n"), "ListboxTop");
          return SET_PROPERTY_ERROR;
        }
    }
  else
    {
      /* Wrong datatype */
      sciprint(_("Wrong type for '%s' property: A real expected.\n"), "ListboxTop");
      return SET_PROPERTY_ERROR;
    }

  /* Store the value in Scilab */
  if (nbCol == 0 || nbRow ==0) /* Empty matrix value */
    {
      if(pUICONTROL_FEATURE(sciObj)->listboxTop != NULL)
        {
          delete [] pUICONTROL_FEATURE(sciObj)->listboxTop;
          pUICONTROL_FEATURE(sciObj)->listboxTop = NULL;
        }
    }
  else
    {
      pUICONTROL_FEATURE(sciObj)->listboxTop = new int[1];
      pUICONTROL_FEATURE(sciObj)->listboxTop[0] = value;
    }
  
  switch(pUICONTROL_FEATURE(sciObj)->style)
    {
    case SCI_LISTBOX:
      // TODO Set the Java property if necessary
      return SET_PROPERTY_SUCCEED;
    default:
      /* No Java attribute to set or method to call */
      return SET_PROPERTY_SUCCEED;
    }
}

