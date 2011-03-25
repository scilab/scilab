/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Han DONG
 * Sets the shear property of uicontrol imagerender 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetUiobjectScale.hxx"

using namespace org_scilab_modules_gui_bridge;

int SetUiobjectScale(sciPointObj* sciObj, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
  double *allValues = NULL;
  int K = 0;
  int nbValue = 0, value = 0;
  
  if (valueType == sci_matrix)
    {
      if(nbRow > 1)
        {
          /* Wrong value size */
          Scierror(999, const_cast<char*>(_("Wrong size for '%s' property: A real row vector expected.\n")), "Value");
          return SET_PROPERTY_ERROR;
        }
      
      /* Store the value in Scilab */
      if (nbCol == 0 || nbRow ==0) /* Empty matrix value */
        {
          if(pUICONTROL_FEATURE(sciObj)->valueSize != 0)
            {
              delete [] pUICONTROL_FEATURE(sciObj)->value;
              pUICONTROL_FEATURE(sciObj)->doubleValue = NULL;
              pUICONTROL_FEATURE(sciObj)->valueSize = 0;
            }
        }
      else
        {
          allValues = getDoubleMatrixFromStack(stackPointer);
          /* TODO memory check ! */
          pUICONTROL_FEATURE(sciObj)->valueSize = nbCol+1;
          pUICONTROL_FEATURE(sciObj)->doubleValue = new double[nbCol+1];
          for (K=0; K<nbCol; K++)
            {
              pUICONTROL_FEATURE(sciObj)->doubleValue[K] = allValues[K];
            }
	  /* Ensures that if user inputs only one variable for x coordinate in scale, it passes an extra 1 for y coordinate */
          pUICONTROL_FEATURE(sciObj)->doubleValue[nbCol] = 1;
        }
    }
  else
    {
      /* Wrong datatype */
      Scierror(999, const_cast<char*>(_("Wrong type for '%s' property: A real row vector expected.\n")), "Value");
      return SET_PROPERTY_ERROR;
    }


  if (allValues != NULL || nbValue==1)
    {
      // Set the Java object property if necessary
      switch(pUICONTROL_FEATURE(sciObj)->style)
        {
        case SCI_IMAGERENDERER:
          if (pUICONTROL_FEATURE(sciObj)->valueSize == 0)
            {
              CallScilabBridge::setImageRendererScale(getScilabJavaVM(), 
                                                          pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                          (double*) pUICONTROL_FEATURE(sciObj)->doubleValue,
                                                          0); /* No scale */
            }
          else
            {
              CallScilabBridge::setImageRendererScale(getScilabJavaVM(), 
                                                          pUICONTROL_FEATURE(sciObj)->hashMapIndex,
                                                          (double*) pUICONTROL_FEATURE(sciObj)->doubleValue,
                                                          pUICONTROL_FEATURE(sciObj)->valueSize);
            }
          return SET_PROPERTY_SUCCEED;
        default:
          /* No Java attribute to set or method to call */
          return SET_PROPERTY_SUCCEED;
        }
    }
  return SET_PROPERTY_ERROR;
  
}

