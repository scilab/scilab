/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Get the value property of an uicontrol */

#include "GetUicontrolValue.hxx"

using namespace org_scilab_modules_gui_bridge;

int GetUicontrolValue(sciPointObj* sciObj)
{
  int * value = NULL;

  int valueSize = 0;

  if (sciGetEntityType(sciObj) == SCI_UICONTROL)
    {
      switch(pUICONTROL_FEATURE(sciObj)->style)
        {
        case SCI_LISTBOX:
          value = (int*) CallScilabBridge::getListBoxSelectedIndices(getScilabJavaVM(), 
                                                              pUICONTROL_FEATURE(sciObj)->hashMapIndex);
          valueSize = CallScilabBridge::getListBoxSelectionSize(getScilabJavaVM(), 
                                                              pUICONTROL_FEATURE(sciObj)->hashMapIndex);
          if (value[0] == -1)
            {
              return sciReturnEmptyMatrix();
            }
          else
            {
              return sciReturnRowVectorFromInt(value, pUICONTROL_FEATURE(sciObj)->valueSize);
            }
        case SCI_POPUPMENU:
          value = (int*) CallScilabBridge::getPopupMenuSelectedIndex(getScilabJavaVM(), 
                                                          pUICONTROL_FEATURE(sciObj)->hashMapIndex);
          if (value[0] == -1)
            {
              return sciReturnEmptyMatrix();
            }
          else
            {
              return sciReturnRowVectorFromInt(value, 1); /* Only one value returned */
            }
        case SCI_SLIDER:
          return sciReturnInt(
                              CallScilabBridge::getSliderValue(getScilabJavaVM(), 
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
      sciprint(_("No %s property for this object.\n"), "Value");
      return FALSE;
    }
}
