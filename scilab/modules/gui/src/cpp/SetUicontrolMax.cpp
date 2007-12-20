/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Sets the max property of an uicontrol object */

#include "SetUicontrolMax.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolMax(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  if (valueType == sci_matrix)
    {
      if(nbCol != 1 || nbRow != 1)
        {
          /* Wrong value size */
          sciprint(_("%s property value must be single value.\n"), "Max");
          return SET_PROPERTY_ERROR;
        }
      
      /* Store the value in Scilab */
      pUICONTROL_FEATURE(sciObj)->max = (int) getDoubleFromStack(stackPointer);
      switch(pUICONTROL_FEATURE(sciObj)->style)
        {
        case SCI_SLIDER:
        case SCI_CHECKBOX:
        case SCI_LISTBOX:
          // TODO Set the Java property if necessary
          return SET_PROPERTY_SUCCEED;
        default:
          /* No Java attribute to set or method to call */
          return SET_PROPERTY_SUCCEED;
        }
    }
  else
    {
      /* Wrong datatype */
      sciprint(_("%s property value must be single value.\n"), "Max");
      return SET_PROPERTY_ERROR;
    }

}

