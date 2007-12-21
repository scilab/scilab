/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Sets the listbox top property of an uicontrol object */

#include "SetUicontrolListboxTop.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolListboxTop(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  if (valueType == sci_matrix)
    {
      if(nbCol > 1 || nbRow > 1)
        {
          /* Wrong value size */
          sciprint(_("%s property value must be single value.\n"), "ListboxTop");
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
          pUICONTROL_FEATURE(sciObj)->listboxTop[0] = (int) getDoubleFromStack(stackPointer);
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
  else
    {
      /* Wrong datatype */
      sciprint(_("%s property value must be single value.\n"), "ListboxTop");
      return SET_PROPERTY_ERROR;
    }

}

