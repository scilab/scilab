/* Copyright INRIA 2007 */
/* Vincent COUVERT */
/* Sets the value property of an uicontrol object */

#include "SetUicontrolValue.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUicontrolValue(sciPointObj* sciObj, int stackPointer, int valueType, int nbRow, int nbCol)
{
  double *allValues = NULL;
  int K = 0;
  
  if (valueType == sci_matrix)
    {
      if(nbRow > 1)
        {
          /* Wrong value size */
          sciprint(_("%s property value must be a row vector.\n"), "Value");
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
      sciprint(_("%s property value must be single value.\n"), "Value");
      return SET_PROPERTY_ERROR;
    }

}

