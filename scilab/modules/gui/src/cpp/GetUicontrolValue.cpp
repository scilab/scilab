/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Get the value property of an uicontrol 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetUicontrolValue.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolValue(sciPointObj* sciObj)
{
  int * value = NULL;

  int singleValue = 0;

  int valueSize = 0;

  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      switch(pUICONTROL_FEATURE(sciObj)->style)
        {
        case SCI_LISTBOX:
		      /* DO A DELETE @ end on value returned by getListBoxSelectedIndices */
          value = (int*) CallScilabBridge::getListBoxSelectedIndices(getScilabJavaVM(), 
                                                              pUICONTROL_FEATURE(sciObj)->hashMapIndex);
          valueSize = CallScilabBridge::getListBoxSelectionSize(getScilabJavaVM(), 
                                                              pUICONTROL_FEATURE(sciObj)->hashMapIndex);
          if (valueSize==0 || value[0] == -1)
            {
              return sciReturnEmptyMatrix();
            }
          else
            {
              if (valueSize == 1)
                {
                  return sciReturnInt(value[0]);
                }
              else
                {
                  return sciReturnRowVectorFromInt(value, valueSize);
                }
            }
        case SCI_POPUPMENU:
          singleValue = (int) CallScilabBridge::getPopupMenuSelectedIndex(getScilabJavaVM(), 
                                                          pUICONTROL_FEATURE(sciObj)->hashMapIndex);
          if (singleValue == -1)
            {
              return sciReturnEmptyMatrix();
            }
          else
            {
              return sciReturnInt(singleValue); /* Only one value returned */
            }
        case SCI_SLIDER:
          return sciReturnInt(CallScilabBridge::getSliderValue(getScilabJavaVM(), 
                                                               pUICONTROL_FEATURE(sciObj)->hashMapIndex)); /* Only one value returned */
        case SCI_CHECKBOX:
          if (CallScilabBridge::isCheckBoxChecked(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex))
            {
              return sciReturnInt(pUICONTROL_FEATURE(sciObj)->max); /* Only one value returned */
            }
          else
            {
              return sciReturnInt(pUICONTROL_FEATURE(sciObj)->min); /* Only one value returned */
            }
        case SCI_RADIOBUTTON:
          if (CallScilabBridge::isRadioButtonChecked(getScilabJavaVM(), pUICONTROL_FEATURE(sciObj)->hashMapIndex))
            {
              return sciReturnInt(pUICONTROL_FEATURE(sciObj)->max); /* Only one value returned */
            }
          else
            {
              return sciReturnInt(pUICONTROL_FEATURE(sciObj)->min); /* Only one value returned */
            }
        default:
          if (pUICONTROL_FEATURE(sciObj)->valueSize == 0)
            {
              return sciReturnEmptyMatrix();
            }
          else
            {
              return sciReturnRowVectorFromInt(pUICONTROL_FEATURE(sciObj)->value, pUICONTROL_FEATURE(sciObj)->valueSize);
            }
        }
    }
  else
    {
      sciprint(_("No '%s' property for this object.\n"), "Value");
      return FALSE;
    }
}
