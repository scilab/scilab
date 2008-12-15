/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Sets the value property of an uicontrol object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUicontrolValue.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolValue(sciPointObj* sciObj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
  double *allValues = NULL;
  int K = 0;
  int nbValue = 0, value = 0;
  
  if (valueType == sci_matrix)
    {
      if(nbRow > 1)
        {
          /* Wrong value size */
          Scierror(999, _("Wrong size for '%s' property: A real row vector expected.\n"), "Value");
          return SET_PROPERTY_ERROR;
        }
      
      /* Store the value in Scilab */
      if (nbCol == 0 || nbRow ==0) /* Empty matrix value */
        {
          if(pUICONTROL_FEATURE(sciObj)->valueSize != 0)
            {
              delete [] pUICONTROL_FEATURE(sciObj)->value;
              pUICONTROL_FEATURE(sciObj)->value = NULL;
              pUICONTROL_FEATURE(sciObj)->valueSize = 0;
            }
        }
      else
        {
          allValues = getDoubleMatrixFromStack(stackPointer);
          /* TODO memoru check ! */
          pUICONTROL_FEATURE(sciObj)->valueSize = nbCol;
          pUICONTROL_FEATURE(sciObj)->value = new int[nbCol];
          for (K=0; K<nbCol; K++)
            {
              pUICONTROL_FEATURE(sciObj)->value[K] = (int) allValues[K];
            }
        }
    }
  else if (valueType == sci_strings) // Ascendant compatibility
    {
      if(nbCol > 1 || nbRow > 1)
        {
          /* Wrong value size */
          Scierror(999, _("Wrong size for '%s' property: A string expected.\n"), "Value");
          return SET_PROPERTY_ERROR;
        }

      nbValue = sscanf(getStringFromStack(stackPointer), "%d", &value);

      if(nbValue != 1)
        {
          /* Wrong value size */
          Scierror(999, _("Wrong value for '%s' property: A string expected.\n"), "Value");
          return SET_PROPERTY_ERROR;
        }

      pUICONTROL_FEATURE(sciObj)->valueSize = 1;
      pUICONTROL_FEATURE(sciObj)->value = new int[1];
      pUICONTROL_FEATURE(sciObj)->value[0] = (int) value;
    }
  else
    {
      /* Wrong datatype */
      Scierror(999, _("Wrong type for '%s' property: A real row vector or a string expected.\n"), "Value");
      return SET_PROPERTY_ERROR;
    }


  if (allValues != NULL || nbValue==1)
    {
      // Set the Java object property if necessary
      switch(pUICONTROL_FEATURE(sciObj)->style)
        {
        case SCI_LISTBOX:
          if (pUICONTROL_FEATURE(sciObj)->valueSize == 0)
            {
              CallScilabBridge::setListBoxSelectedIndices(getScilabJavaVM(), 
                                                          pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                          (int*) pUICONTROL_FEATURE(sciObj)->value,
                                                          -1); /* No value selected */
            }
          else
            {
              CallScilabBridge::setListBoxSelectedIndices(getScilabJavaVM(), 
                                                          pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                          (int*) pUICONTROL_FEATURE(sciObj)->value,
                                                          pUICONTROL_FEATURE(sciObj)->valueSize);
            }
          return SET_PROPERTY_SUCCEED;
        case SCI_POPUPMENU:
          if (pUICONTROL_FEATURE(sciObj)->valueSize != 1)
            {
              /* Wrong value size */
              Scierror(999, _("Wrong size for '%s' property: A real expected.\n"), "Value");
              return SET_PROPERTY_ERROR;
            }
          else
            {
              CallScilabBridge::setPopupMenuSelectedIndex(getScilabJavaVM(), 
                                                          pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                          pUICONTROL_FEATURE(sciObj)->value[0]);
              return SET_PROPERTY_SUCCEED;
            }
        case SCI_CHECKBOX:
          if (pUICONTROL_FEATURE(sciObj)->valueSize != 0)
            {
              if ((pUICONTROL_FEATURE(sciObj)->value[0] != pUICONTROL_FEATURE(sciObj)->min) && (pUICONTROL_FEATURE(sciObj)->value[0] != pUICONTROL_FEATURE(sciObj)->max))
                {
                  sciprint(_("Warning: '%s' 'Value' property should be equal to either '%s' or '%s' property value.\n"), "Checkbox", "Min", "Max");
               }

              CallScilabBridge::setCheckBoxChecked(getScilabJavaVM(), 
                                                   pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                   pUICONTROL_FEATURE(sciObj)->value[0] == pUICONTROL_FEATURE(sciObj)->max);
            }
          return SET_PROPERTY_SUCCEED;
        case SCI_RADIOBUTTON:
          if (pUICONTROL_FEATURE(sciObj)->valueSize != 0)
            {
              if ((pUICONTROL_FEATURE(sciObj)->value[0] != pUICONTROL_FEATURE(sciObj)->min) && (pUICONTROL_FEATURE(sciObj)->value[0] != pUICONTROL_FEATURE(sciObj)->max))
                {
                  sciprint(_("Warning: '%s' 'Value' property should be equal to either '%s' or '%s' property value.\n"), "RadioButton", "Min", "Max");
               }

              CallScilabBridge::setRadioButtonChecked(getScilabJavaVM(), 
                                                      pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                      pUICONTROL_FEATURE(sciObj)->value[0] == pUICONTROL_FEATURE(sciObj)->max);
            }
          return SET_PROPERTY_SUCCEED;
        case SCI_SLIDER:
          if (pUICONTROL_FEATURE(sciObj)->valueSize != 0)
            {
              CallScilabBridge::setSliderValue(getScilabJavaVM(), 
                                               pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                               pUICONTROL_FEATURE(sciObj)->value[0]);
            }
          return SET_PROPERTY_SUCCEED;
        default:
          /* No Java attribute to set or method to call */
          return SET_PROPERTY_SUCCEED;
        }
    }
  return SET_PROPERTY_ERROR;
  
}

