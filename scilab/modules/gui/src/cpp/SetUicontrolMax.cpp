/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Sets the max property of an uicontrol object */

#include "SetUicontrolMax.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolMax(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  if (valueType == sci_matrix)
    {
      int value = 0;

      if(nbCol != 1 || nbRow != 1)
        {
          /* Wrong value size */
          sciprint(_("%s property value must be single value.\n"), "Max");
          return SET_PROPERTY_ERROR;
        }
      
      value = (int) getDoubleFromStack(stackPointer);

      /* Check if value is < to max property */
      if (value > pUICONTROL_FEATURE(sciObj)->min)
        {
          /* Store the value in Scilab */
          pUICONTROL_FEATURE(sciObj)->max = value;
        }
      else
        {
          /* Wrong value size */
          sciprint(_("%s property value must be greater than Min property value.\n"), "Max");
          return SET_PROPERTY_ERROR;
        }

      /* Update Java Objects */
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_SLIDER)
        {
          /* Max value */
          CallScilabBridge::setSliderMaxValue(getScilabJavaVM(),
                                                      pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                      (int) pUICONTROL_FEATURE(sciObj)->max);
          /* Ticks spacing */
          CallScilabBridge::setSliderMinorTickSpacing(getScilabJavaVM(),
                                                      pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                      (int) pUICONTROL_FEATURE(sciObj)->sliderStep[0] * (pUICONTROL_FEATURE(sciObj)->max - pUICONTROL_FEATURE(sciObj)->min));
          
          CallScilabBridge::setSliderMajorTickSpacing(getScilabJavaVM(), 
                                                      pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                      (int) pUICONTROL_FEATURE(sciObj)->sliderStep[1] * (pUICONTROL_FEATURE(sciObj)->max - pUICONTROL_FEATURE(sciObj)->min));
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
          // TODO
        }
      else if (pUICONTROL_FEATURE(sciObj)->style == SCI_RADIOBUTTON)
        {
          // TODO
        }
      return SET_PROPERTY_SUCCEED;
    }
  else
    {
      /* Wrong datatype */
      sciprint(_("%s property value must be single value.\n"), "Max");
      return SET_PROPERTY_ERROR;
    }

}

