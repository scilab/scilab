/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Sets the min property of an uicontrol object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUicontrolMin.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolMin(sciPointObj* sciObj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
  if (valueType == sci_matrix)
    {

      if(nbCol != 1 || nbRow != 1)
        {
          /* Wrong value size */
          sciprint(_("Wrong size for '%s' property: A real expected.\n"), "Min");
          return SET_PROPERTY_ERROR;
        }

      /* Store the value in Scilab */
      pUICONTROL_FEATURE(sciObj)->min = (int) getDoubleFromStack(stackPointer);

      /* Update Java Objects */
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_SLIDER)
        {
          /* Min value */
          CallScilabBridge::setSliderMinValue(getScilabJavaVM(),
                                                      pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                      (int) pUICONTROL_FEATURE(sciObj)->min);
         
          /* Ticks spacing: if not user defined */
          if (pUICONTROL_FEATURE(sciObj)->sliderStep == NULL)
            {
              /* Check if min is < to max property */
              if (pUICONTROL_FEATURE(sciObj)->min <= pUICONTROL_FEATURE(sciObj)->max)
                {
                  CallScilabBridge::setSliderMinorTickSpacing(getScilabJavaVM(),
                                                              pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                              (int) (0.01 * (pUICONTROL_FEATURE(sciObj)->max - pUICONTROL_FEATURE(sciObj)->min)));
                  
                  CallScilabBridge::setSliderMajorTickSpacing(getScilabJavaVM(), 
                                                              pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                              (int) (0.1 * (pUICONTROL_FEATURE(sciObj)->max - pUICONTROL_FEATURE(sciObj)->min)));
                }
            }
       }
      else if (pUICONTROL_FEATURE(sciObj)->style == SCI_LISTBOX)
        {
          /* Multiselection available ? */
          if ((pUICONTROL_FEATURE(sciObj)->max - pUICONTROL_FEATURE(sciObj)->min) > 1)
            {
              CallScilabBridge::setListBoxMultipleSelectionEnabled(getScilabJavaVM(),
                                                                   pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                                   TRUE);
            }
          else
            {
              CallScilabBridge::setListBoxMultipleSelectionEnabled(getScilabJavaVM(),
                                                                   pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                                   FALSE);
            }
        }
      else if (pUICONTROL_FEATURE(sciObj)->style == SCI_CHECKBOX)
        {
          if (pUICONTROL_FEATURE(sciObj)->valueSize != 0) /* Value set by the user */
            {
              if ((pUICONTROL_FEATURE(sciObj)->value[0] != pUICONTROL_FEATURE(sciObj)->min) && (pUICONTROL_FEATURE(sciObj)->value[0] != pUICONTROL_FEATURE(sciObj)->max))
                {
                  sciprint(_("Warning: '%s' 'Value' property should be equal to either '%s' or '%s' property value.\n"), "Checkbox", "Min", "Max");
                }

              CallScilabBridge::setCheckBoxChecked(getScilabJavaVM(), 
                                                   pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                   pUICONTROL_FEATURE(sciObj)->value[0] == pUICONTROL_FEATURE(sciObj)->max);
            }
          else  /* Default Value is 0 */
            {
              CallScilabBridge::setCheckBoxChecked(getScilabJavaVM(), 
                                                   pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                   pUICONTROL_FEATURE(sciObj)->max == 0);
            }
        }
      else if (pUICONTROL_FEATURE(sciObj)->style == SCI_RADIOBUTTON)
        {
          if (pUICONTROL_FEATURE(sciObj)->valueSize != 0)  /* Value set by the user */
            {
              if ((pUICONTROL_FEATURE(sciObj)->value[0] != pUICONTROL_FEATURE(sciObj)->min) && (pUICONTROL_FEATURE(sciObj)->value[0] != pUICONTROL_FEATURE(sciObj)->max))
                {
                  sciprint(_("Warning: '%s' 'Value' property should be equal to either '%s' or '%s' property value.\n"), "Radiobutton", "Min", "Max");
                }

              CallScilabBridge::setRadioButtonChecked(getScilabJavaVM(), 
                                                      pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                      pUICONTROL_FEATURE(sciObj)->value[0] == pUICONTROL_FEATURE(sciObj)->max);
            }
          else  /* Default Value is 0 */
            {
              CallScilabBridge::setRadioButtonChecked(getScilabJavaVM(), 
                                                      pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                      pUICONTROL_FEATURE(sciObj)->max == 0);
            }
        }
      return SET_PROPERTY_SUCCEED;
    }
  else
    {
      /* Wrong datatype */
      sciprint(_("Wrong type for '%s' property: A real expected.\n"), "Min");
      return SET_PROPERTY_ERROR;
    }

}

